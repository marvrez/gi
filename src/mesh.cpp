#include "mesh.h"

#include "kdtree.h"
#include "vec3.h"
#include "mat4.h"
#include "material.h"
#include "surface.h"

#include <unordered_map>
#include <assert.h>

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
    return this->tree->Intersect(r, h);
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
    if(!this->tree) {
        std::vector<Surface*> surfaces;
        surfaces.reserve(this->num_triangles);
        for(Triangle& t : this->triangles) {
            surfaces.push_back(&t);
        }
        this->tree = std::make_unique<KDTree>(surfaces);
    }
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
