#include "scene.h"

#include "ray.h"
#include "kdtree.h"
#include "surface.h"

void Scene::Add(Surface* s)
{
    this->surfaces.push_back(s);
    if(s->Emittable()) this->lights.push_back(s);
}

bool Scene::Intersect(const Ray& r, Hit* h)
{
    this->ray_count++;
    return this->tree->Intersect(r, h);
}

void Scene::Build()
{
    if(this->tree == nullptr) {
        this->tree = std::make_unique<KDTree>(this->surfaces);
    }
}
