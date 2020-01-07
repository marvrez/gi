#ifndef KDTREE_H
#define KDTREE_H

#include "bbox.h"
#include "utils.h"

#include <vector>

struct Hit;
class Surface;

constexpr double BALANCING_FACTOR = 0.85;

struct KDNode {
    Axis axis;
    double pos;
    std::vector<Surface*> surfaces;
    std::unique_ptr<KDNode> left, right;

    KDNode() {};
    KDNode(const std::vector<Surface*>& surfaces) : axis(Axis::NONE), pos(0), surfaces(surfaces), left(nullptr), right(nullptr) { }

    void Split(int depth);
    bool Intersect(const Ray& r, Hit* h);
    bool Intersect(const Ray& r, double tmin, double tmax, Hit* h);
    int Partition(Axis axis, double p, int size, std::vector<Surface*>* left, std::vector<Surface*>* right);
    int PartitionCount(Axis axis, double p) { return Partition(axis, p, 0, 0, 0); }
};

class KDTree {
private:
    BBox bbox;
    std::unique_ptr<KDNode> root;

public:
    KDTree(const std::vector<Surface*>& surfaces);
    bool Intersect(const Ray& r, Hit* h);
};

#endif
