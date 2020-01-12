#ifndef SCENE_H
#define SCENE_H

#include "kdtree.h"

#include <vector>
#include <memory>
#include <atomic>

class Surface;
struct Hit;
struct Ray;

class Scene {
private:
    std::vector<Surface*> surfaces;
    std::vector<Surface*> lights;
    std::unique_ptr<KDTree> tree;
    std::atomic<unsigned> ray_count;

public:
    Scene() : ray_count(0) {};
    void Add(Surface* s);
    bool Intersect(const Ray& r, Hit* h);
    void Build();
    unsigned RayCount() const { return this->ray_count.load(); }
    void ResetRayCount() { this->ray_count = 0; }
};

#endif
