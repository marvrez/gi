#ifndef RAY_H
#define RAY_H

#include "vec3.h"

struct Ray {
    Vec3 origin, direction;
    Ray() {};
    Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction) {}

    inline Vec3 PositionAt(double t) const { return origin + t*direction; }

    // calculate reflecting ray given the position and normal
    Ray Reflect(const Vec3& position, const Vec3& normal) const;
    Ray Refract(const Vec3& position, const Vec3& normal, double n1, double n2) const;
};

#endif
