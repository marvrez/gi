#ifndef SCENE_H
#define SCENE_H

#include "kdtree.h"

#include <vector>
#include <memory>

class Surface;
struct Hit;
struct Ray;

class Scene {
private:
    std::vector<Surface*> surfaces;
    std::unique_ptr<KDTree> tree;

public:
    Scene() {};
    void Add(Surface* s);
    bool Intersect(const Ray& r, Hit* h);
    void Build();
};

#endif
