#ifndef CUBE_H
#define CUBE_H

#include "vec3.h"
#include "bbox.h"
#include "surface.h"

class Material;
struct Hit;

class Cube : public Surface {
private:
    Vec3 min_point, max_point;
    std::shared_ptr<Material> material;
    BBox bbox;
public:
    Cube(const Vec3& min_point, const Vec3& max_point, Material* material);
    virtual BBox GetBBox();
    virtual bool Intersect(const Ray& r, Hit* h) const;
    virtual Vec3 UV(const Vec3& p) const;
    virtual Vec3 NormalAt(const Vec3& p) const;
    virtual Material* MaterialAt(const Vec3& p) const;
};

#endif
