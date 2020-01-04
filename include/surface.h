#ifndef SHAPE_H
#define SHAPE_H

#include "vec3.h"
#include "material.h"

struct BBox;
struct Ray;
struct Hit;

class Surface {
public:
    virtual BBox GetBBox() = 0;
    virtual bool Intersect(const Ray& r, Hit* h) = 0;
    virtual Vec3 UV(const Vec3& p) const { return {}; }
    virtual Vec3 NormalAt(const Vec3& p) const { return {}; }
    virtual Material MaterialAt(const Vec3& p) const { return {}; }
};

#endif
