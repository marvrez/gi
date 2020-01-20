#ifndef SHAPE_H
#define SHAPE_H

#include "vec3.h"
#include "ray.h"

#include <memory>

struct BBox;
struct Ray;
struct Hit;
class Material;

class Surface {
public:
    virtual BBox GetBBox() = 0;
    virtual bool Intersect(const Ray& r, Hit* h) const = 0;
    virtual Vec3 UV(const Vec3& p) const { return {}; }
    virtual Vec3 NormalAt(const Vec3& p) const { return {}; }
    virtual Material* MaterialAt(const Vec3& p) const { return nullptr; }
    virtual bool Emittable() const { return false; }
    virtual Ray RandomRay(const Vec3& hit_point) const { return {}; }
    virtual double Pdf(const Ray& r) const { return 0; }
    virtual void Build() {}
};

#endif
