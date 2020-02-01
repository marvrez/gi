#include "cube.h"

#include "surface.h"
#include "material.h"
#include "hit.h"

Cube::Cube(const Vec3& min_point, const Vec3& max_point, Material* material)
    : min_point(min_point), max_point(max_point), material(material), bbox(min_point, max_point)
{
}

BBox Cube::GetBBox()
{
    return this->bbox;
}

bool Cube::Intersect(const Ray& r, Hit* h) const
{
    Vec3 near = (this->min_point - r.origin) / r.direction;
    Vec3 far = (this->max_point - r.origin) / r.direction;
    Vec3 min = Min(near, far), max = Max(near, far);
    double t0 = min.MaxComponent(), t1 = max.MinComponent();
    if(t0 > t1) return false;
    double t, tmin = M_EPS, tmax = h->t;
    if(t0 < tmax && t0 > tmin) t = t0;
    else if(t1 < tmax && t1 > tmin) t = t1;
    else return false;
    h->RecordHit(t, this);
    return true;
}

Vec3 Cube::UV(const Vec3& p) const
{
    Vec3 uv = (p - this->min_point) / (this->max_point - this->min_point);
    return { uv.u, uv.v, 0 };
}

Vec3 Cube::NormalAt(const Vec3& p) const
{
    if(p.x < min_point.x + M_EPS) return Vec3(-1, 0, 0);
    if(p.x > max_point.x - M_EPS) return Vec3(1, 0, 0);
    if(p.y < min_point.y + M_EPS) return Vec3(0, -1, 0);
    if(p.y > max_point.y - M_EPS) return Vec3(0, 1, 0);
    if(p.z < min_point.z + M_EPS) return Vec3(0, 0, -1);
    if(p.z > max_point.z - M_EPS) return Vec3(0, 0, 1);
    return Vec3(0, 1, 0);
}

Material* Cube::MaterialAt(const Vec3& p) const
{
    return this->material.get();
}
