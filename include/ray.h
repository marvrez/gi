#ifndef RAY_H
#define RAY_H

#include "vec3.h"

struct Ray {
    Vec3 origin, direction;
    Ray() {};
    Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction) {}

    inline Vec3 PositionAt(double t) const { return origin + t*direction; }
};

#endif
