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

    BBox Union(const BBox& b) const;
    bool Contains(const Vec3& p) const;
    bool Intersects(const BBox& b)  const;
    bool Intersect(const Ray& r, double* t1, double* t2) const;
    Vec3 Size() const;
    void Partition(Axis axis, double p, bool* left, bool* right) const;
};

BBox SurroundingBBox(const std::vector<Surface*>& surfaces);

#endif
