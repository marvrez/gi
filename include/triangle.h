#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "surface.h"

#include <memory>

class Material;
class Mesh;

class Triangle : public Surface {
private:
    friend class Mesh;
    Mesh* mesh; // pointer to parent mesh that stores all data
    const int* v; // face indices
    Material* material;

    Vec3 Barycentric(const Vec3& pos) const;
public:
    Triangle(Mesh* mesh, int face_idx, Material* material);

    virtual BBox GetBBox();
    virtual bool Intersect(const Ray& r, Hit* h) const;
    virtual Vec3 UV(const Vec3& p) const;
    virtual Vec3 NormalAt(const Vec3& p) const;
    virtual Material* MaterialAt(const Vec3& p) const;
    virtual bool Emittable() const;

    Vec3 Normal() const;
};

#endif
