#ifndef BOX_H
#define BOX_H

#include "vec3.h"
#include "utils.h"

#include <vector>
#include <memory>

struct Ray;
class Surface;

// Axis Aligned Bounding Box
struct BBox {
    Vec3 min_point;
    Vec3 max_point;

    BBox() = default;
    BBox(const Vec3& min, const Vec3& max) : min_point(min), max_point(max) {}

    BBox Union(const BBox& b) const;
    bool Contains(const Vec3& p) const;
    bool Intersects(const BBox& b)  const;
    bool Intersect(const Ray& r, double* t1, double* t2) const;
    Vec3 Size() const;
    void Partition(Axis axis, double p, bool* left, bool* right) const;

    Vec3 Anchor(const Vec3& anchor) const;
};

BBox SurroundingBBox(const std::vector<Surface*>& surfaces);

#endif
