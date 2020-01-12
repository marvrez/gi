#ifndef ONB_H
#define ONB_H

#include "vec3.h"

// orthonormal basis
class ONB {
private:
    Vec3 u, v, w; // orthonormal basis vectors

public:
    ONB(const Vec3& normal);

    Vec3 WorldToLocal(const Vec3& p) const;
    Vec3 LocalToWorld(const Vec3& p) const;
};

#endif
