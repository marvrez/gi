#include "ray.h"

static inline Vec3 ReflectVec(const Vec3& v, const Vec3& n)
{
    return v - 2.f*Dot(v, n)*n;
}

static inline Vec3 RefractVec(const Vec3& v, const Vec3& n, float ratio)
{
    float dt = Dot(v, n);
    float discriminant = 1.f - ratio*ratio*(1.f - dt*dt);
    if(discriminant <= 0.f) return {};
    return ratio*(v - n*dt) - n*sqrtf(discriminant);
}

Ray Ray::Reflect(const Vec3& position, const Vec3& normal) const
{
    Vec3 reflect_dir = ReflectVec(this->direction, normal);
    return { position, reflect_dir };
}

Ray Ray::Refract(const Vec3& position, const Vec3& normal, float n1, float n2) const
{
    float ratio = n1 / n2;
    Vec3 refract_dir = RefractVec(this->direction, normal, ratio);
    return { position, refract_dir };
}
