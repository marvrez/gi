#include "ray.h"

static inline Vec3 ReflectVec(const Vec3& v, const Vec3& n)
{
    return v - 2*Dot(v, n)*n;
}

static inline Vec3 RefractVec(const Vec3& v, const Vec3& n, double ratio)
{
    double dt = Dot(v, n);
    double discriminant = 1.0 - ratio*ratio*(1.0 - dt*dt);
    if(discriminant <= 0.0) return Vec3(0);
    return ratio*(v - n*dt) - n*sqrt(discriminant);
}

Ray Ray::Reflect(const Vec3& position, const Vec3& normal) const
{
    Vec3 reflect_dir = ReflectVec(this->direction, normal);
    return { position, reflect_dir };
}

Ray Ray::Refract(const Vec3& position, const Vec3& normal, double n1, double n2) const
{
    double ratio = n1 / n2;
    Vec3 refract_dir = RefractVec(this->direction, normal, ratio);
    return { position, refract_dir };
}
