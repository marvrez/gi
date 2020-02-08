#include "mesh.h"

#include "kdtree.h"
#include "vec3.h"
#include "mat4.h"
#include "hit.h"
#include "material.h"
#include "surface.h"

#include <unordered_map>
#include <assert.h>

#ifdef EMBREE
RTCDevice device = rtcNewDevice(NULL);

struct EmbreeVertex   { float x, y, z; };
struct EmbreeTriangle { int v0, v1, v2; };

#endif

Mesh::Mesh(const std::vector<Vec3>& positions, Material* material)
    : Mesh(positions, {}, {}, material)
{
}

Mesh::Mesh(const std::vector<Vec3>& positions, const std::vector<Vec3>& normals, Material* material)
    : Mesh(positions, normals, {}, material)
{
}

Mesh::Mesh(const std::vector<Vec3>& positions, const std::vector<Vec3>& normals, const std::vector<Vec3>& texcoords, Material* material)
    : num_triangles(positions.size() / 3)
{
    assert(positions.size() > 0);
    if(normals.size() > 0) assert(normals.size() == positions.size());
    if(texcoords.size() > 0) assert(texcoords.size() == positions.size());
    // remove duplicate vertices
    std::unordered_map<Vec3, int> map; // mapping: position --> index in positions array
    int cur_size = 0;
    for(unsigned i = 0; i < positions.size(); ++i) {
        const Vec3& p = positions[i];
        if(map.find(p) == map.end()) {
            map[p] = cur_size++;
            this->positions.push_back(p);
            if(normals.size() > 0) this->normals.push_back(normals[i]);
            if(texcoords.size() > 0) this->texcoords.push_back(texcoords[i]);
        }
    }
    // create index array
    for(int i = 0; i < this->num_triangles; ++i) {
        this->indices.push_back(map[positions[3*i + 0]]);
        this->indices.push_back(map[positions[3*i + 1]]);
        this->indices.push_back(map[positions[3*i + 2]]);
    }
    for(int i = 0; i < this->num_triangles; ++i) this->triangles.emplace_back(this, i, material);
    this->RepairNormals();
}

void Mesh::Dirtify()
{
    this->bbox.release();
    this->tree.release();
}

bool Mesh::Intersect(const Ray& r, Hit* h) const
{
#ifdef EMBREE
    RTCIntersectContext context;
    rtcInitIntersectContext(&context);

    Vec3 o = r.origin, d = r.direction;

    RTCRayHit rtc_rh;
    rtc_rh.ray.org_x = o.x; rtc_rh.ray.org_y = o.y; rtc_rh.ray.org_z = o.z;
    rtc_rh.ray.dir_x = d.x; rtc_rh.ray.dir_y = d.y; rtc_rh.ray.dir_z = d.z;
    rtc_rh.ray.tnear = M_EPS; rtc_rh.ray.tfar = h->t;
    rtc_rh.ray.mask = -1;
    rtc_rh.ray.flags = rtc_rh.ray.time = rtc_rh.ray.id = 0;

    rtc_rh.hit.geomID = rtc_rh.hit.primID = RTC_INVALID_GEOMETRY_ID;

    rtcIntersect1(this->embree_scene, &context, &rtc_rh);

    if(rtc_rh.hit.primID == RTC_INVALID_GEOMETRY_ID) return false;

    double t = rtc_rh.ray.tfar;
    h->RecordHit(t, &this->triangles[rtc_rh.hit.primID]);

    return true;
#else
    return this->tree->Intersect(r, h);
#endif
}

void Mesh::Build()
{
    if(!this->bbox) {
        Vec3 min = this->positions[0], max = this->positions[0];
        for(const Vec3& pos : this->positions) {
            min = Min(min, pos), max = Max(max, pos);
        }
        this->bbox = std::make_unique<BBox>(min, max);
    }
#ifdef EMBREE
    this->embree_scene = rtcNewScene(device);
    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

    EmbreeVertex* vbuf = (EmbreeVertex*)rtcSetNewGeometryBuffer(
        geom, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3,
        sizeof(EmbreeVertex), this->positions.size()
    );
    for(int i = 0; i < this->positions.size(); ++i) {
        const Vec3& p = this->positions[i];
        vbuf[i] = { float(p.x), float(p.y), float(p.z) };
    }

    EmbreeTriangle* tbuf = (EmbreeTriangle*)rtcSetNewGeometryBuffer(
        geom, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3,
        sizeof(EmbreeTriangle), this->triangles.size()
    );
    for(int i = 0; i < this->triangles.size(); ++i) {
        const Triangle& t = this->triangles[i];
        tbuf[i] = { t.v[0], t.v[1], t.v[2] };
    }

    rtcCommitGeometry(geom);
    rtcAttachGeometry(this->embree_scene, geom);
    rtcReleaseGeometry(geom);
    rtcCommitScene(this->embree_scene);
#else
    if(!this->tree) {
        std::vector<Surface*> surfaces;
        surfaces.reserve(this->num_triangles);
        for(Triangle& t : this->triangles) {
            surfaces.push_back(&t);
        }
        this->tree = std::make_unique<KDTree>(surfaces);
    }
#endif
}

BBox Mesh::GetBBox()
{
    if(!this->bbox) this->Build();
    return *this->bbox;
}

void Mesh::RepairNormals()
{
    if(!this->normals.empty()) return;
    this->normals.resize(this->positions.size(), Vec3(0));
    for(const Triangle& t : this->triangles) {
        Vec3 n = t.Normal();
        this->normals[t.v[0]] += n;
        this->normals[t.v[1]] += n;
        this->normals[t.v[2]] += n;
    }
    for(Vec3& n : this->normals) n = Normalized(n);
}

void Mesh::Transform(const Mat4& m)
{
    for(Vec3& p : this->positions)  p = m.MultiplyPosition(p);
    for(Vec3& n : this->normals)    n = m.MultiplyDirection(n);
    this->Dirtify();
}

void Mesh::Rotate(const Vec3& axis, double angle_rad)
{
    this->Transform(RotationMatrix(axis, angle_rad));
}

void Mesh::MoveTo(const Vec3& pos, const Vec3& anchor)
{
    Mat4 m = TranslationMatrix(pos - this->GetBBox().Anchor(anchor));
    this->Transform(m);
}

void Mesh::MoveToCentre()
{
    this->MoveTo(Vec3(0), Vec3(0.5));
}

void Mesh::FitInside(const BBox& box, const Vec3& anchor)
{
    BBox bbox = this->GetBBox();
    double scale = (box.Size() / bbox.Size()).MinComponent();
    Vec3 rest = box.Size() - scale*bbox.Size(); // remainder of the original bbox that must be fit
    Mat4 m = IdentityMatrix();
    m = m.Translate(-bbox.min_point); // translate mesh to origin
    m = m.Scale(Vec3(scale)); // scale mesh to desired size
    m = m.Translate(box.min_point + rest*anchor); // translate mesh to be inside desired bbox and add remainder
    this->Transform(m);
}

void Mesh::FitInsideUnitCube()
{
    BBox b = BBox(Vec3(-0.5), Vec3(0.5));
    this->FitInside(b, Vec3(0.5));
}

void Mesh::FitInsideBiUnitCube()
{
    BBox b = BBox(Vec3(-1), Vec3(1));
    this->FitInside(b, Vec3(0.5));
}
