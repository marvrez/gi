#include "plane.h"

#include "ray.h"
#include "hit.h"

Plane::Plane(Vec3 point, Vec3 normal, Material material)
    : point(point), material(material)
{
    this->normal = normal.Normalized();
    this->bbox = { Vec3(-M_INF), Vec3(M_INF) };
}

BBox Plane::GetBBox()
{
    return this->bbox;
}

bool Plane::Intersect(const Ray& r, Hit* h)
{
    double d = Dot(this->normal, r.direction);
    if(abs(d) < M_EPS) return false;
    Vec3 op = this->point - r.origin;
    double t = Dot(op, this->normal) / d, tmax = h->t;
    if(t < M_EPS || t > tmax) return false;
    h->RecordHit(t, this);
    return true;
}

Vec3 Plane::NormalAt(const Vec3& p) const
{
    return this->normal;
}

Material Plane::MaterialAt(const Vec3& p) const
{
    return this->material;
}
