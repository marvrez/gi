#include "sphere.h"

#include "ray.h"
#include "hit.h"

#include <math.h>

Sphere::Sphere(Vec3 centre, double radius, Material material)
    : centre(centre), radius(radius), material(material)
{
    Vec3 min_point = {centre.x - radius, centre.y - radius, centre.z - radius};
    Vec3 max_point = {centre.x + radius, centre.y + radius, centre.z + radius};
    this->bbox = { min_point, max_point };
}

BBox Sphere::GetBBox()
{
    return this->bbox;
}

bool Sphere::Intersect(const Ray& r, Hit* h)
{
    Vec3 to = r.origin - this->centre;
    double b = Dot(to, r.direction);
    double c = Dot(to, to) - this->radius*this->radius;
    double d = b*b - c; // discriminant
    if(d > 0.) {
        d = sqrt(d);
        double t = -b - d, tmax = h->t;
        if(t > M_EPS && t < tmax) {
            h->RecordHit(t, this);
            return true;
        }
        t = -b + d;
        if(t > M_EPS && t < tmax) {
            h->RecordHit(t, this);
            return true;
        }
    }
    return false;
}

Vec3 Sphere::UV(const Vec3& p) const
{
    Vec3 c = (p - this->centre) / this->radius; // unit sphere coordinates at origin
    double phi = atan2(c.z, c.x), theta = asin(c.y);
    double u = 1 - (phi + M_PI) / (2*M_PI);
    double v = (theta + M_PI_2) / M_PI;
    return {u, v, 0.};
}

Vec3 Sphere::NormalAt(const Vec3& p) const
{
    return (p - this->centre).Normalized();
}

Material Sphere::MaterialAt(const Vec3& p) const
{
    return this->material;
}
