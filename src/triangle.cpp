#include "triangle.h"

#include "material.h"
#include "hit.h"
#include "bbox.h"
#include "mesh.h"

Triangle::Triangle(Mesh* mesh, int face_idx, Material* material)
    : mesh(mesh), material(material)
{
    this->v = &mesh->indices[3*face_idx];
}

Vec3 Triangle::Barycentric(const Vec3& pos) const
{
    const Vec3& v0 = mesh->positions[v[0]];
    const Vec3& v1 = mesh->positions[v[1]];
    const Vec3& v2 = mesh->positions[v[2]];
    Vec3 e0 = v1 - v0, e1 = v2 - v0, e2 = pos - v0;
    double d00 = Dot(e0, e0);
    double d01 = Dot(e0, e1), d11 = Dot(e1, e1);
    double d20 = Dot(e2, e0), d21 = Dot(e2, e1);
    double d = d00*d11 - d01*d01;
    double v = (d11*d20 - d01*d21) / d;
    double w = (d00*d21 - d01*d20) / d;
    double u = 1.0 - v - w;
    return { u, v, w };
}

BBox Triangle::GetBBox()
{
    const Vec3& v0 = mesh->positions[v[0]];
    const Vec3& v1 = mesh->positions[v[1]];
    const Vec3& v2 = mesh->positions[v[2]];
    Vec3 min = Min(v0, Min(v1, v2));
    Vec3 max = Max(v0, Max(v1, v2));
    return { min, max };
}

bool Triangle::Intersect(const Ray& r, Hit* h) const
{
    const Vec3& v0 = mesh->positions[v[0]];
    const Vec3& v1 = mesh->positions[v[1]];
    const Vec3& v2 = mesh->positions[v[2]];
    Vec3 e1 = v1 - v0, e2 = v2 - v0;
    Vec3 p = Cross(r.direction, e2);
    double det = Dot(e1, p);
    if(det > -M_EPS && det < M_EPS) return false;
    double inv_det = 1.0 / det;
    Vec3 t = r.origin - v0;
    double u = Dot(t, p)*inv_det;
    if(u < 0 || u > 1) return false;
    Vec3 q = Cross(t, e1);
    double v = Dot(r.direction, q)*inv_det;
    if(v < 0 || u + v > 1) return false;
    double d = Dot(e2, q)*inv_det;
    if(d < M_EPS || d > h->t) return false;
    h->RecordHit(d, this);
    return true;
}

Vec3 Triangle::UV(const Vec3& p) const
{
    if(mesh->texcoords.empty()) return {};
    const Vec3& t0 = mesh->texcoords[v[0]];
    const Vec3& t1 = mesh->texcoords[v[1]];
    const Vec3& t2 = mesh->texcoords[v[2]];
    Vec3 b = this->Barycentric(p);
    Vec3 n = b.u*t0 + b.v*t1 + b.w*t2;
    return { n.x, n.y, 0.0 };
}

Vec3 Triangle::NormalAt(const Vec3& p) const
{
    Vec3 b = this->Barycentric(p);
    const Vec3& n0 = mesh->normals[v[0]];
    const Vec3& n1 = mesh->normals[v[1]];
    const Vec3& n2 = mesh->normals[v[2]];
    return Normalized(b.u*n0 + b.v*n1 + b.w*n2);
}

Material* Triangle::MaterialAt(const Vec3& p) const
{
    return this->material;
}

bool Triangle::Emittable() const
{
    return this->material->Emittable();
}

Vec3 Triangle::Normal() const
{
    const Vec3& v0 = mesh->positions[v[0]];
    const Vec3& v1 = mesh->positions[v[1]];
    const Vec3& v2 = mesh->positions[v[2]];
    return Normalized(Cross(v1 - v0, v2 - v0));
}
