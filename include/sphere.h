#ifndef SPHERE_H
#define SPHERE_H

#include "surface.h"
#include "material.h"
#include "bbox.h"

class Sphere : public Surface {
private:
    Vec3 centre;
    double radius;
    std::shared_ptr<Material> material;
    BBox bbox;

public:
    Sphere(Vec3 centre, double radius, Material* material);

    virtual BBox GetBBox() const;
    virtual bool Intersect(const Ray& r, Hit* h) const;
    virtual Vec3 UV(const Vec3& p) const;
    virtual Vec3 NormalAt(const Vec3& p) const;
    virtual Material* MaterialAt(const Vec3& p) const;
    virtual bool Emittable() const;
    virtual Ray RandomRay(const Vec3& hit_point) const;
    virtual double Pdf(const Ray& r) const;
};

#endif
