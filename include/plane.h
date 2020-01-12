#ifndef PLANE_H
#define PLANE_H

#include "surface.h"
#include "material.h"
#include "bbox.h"

#include <memory>

class Plane : public Surface {
private:
    Vec3 point, normal;
    std::shared_ptr<Material> material;
    BBox bbox;

public:
    Plane(Vec3 point, Vec3 normal, Material* material);

    virtual BBox GetBBox();
    virtual bool Intersect(const Ray& r, Hit* h);
    virtual Vec3 NormalAt(const Vec3& p) const;
    virtual Material* MaterialAt(const Vec3& p) const;
    virtual bool Emittable() const;
};

#endif
