#include "scene.h"

#include "ray.h"
#include "kdtree.h"
#include "surface.h"

static thread_local unsigned ray_count;

void Scene::Add(Surface* s)
{
    this->surfaces.push_back(s);
    if(s->Emittable()) this->lights.push_back(s);
}

bool Scene::Intersect(const Ray& r, Hit* h)
{
    ray_count++;
    return this->tree->Intersect(r, h);
}

void Scene::Build()
{
    if(this->tree == nullptr) {
        this->tree = std::make_unique<KDTree>(this->surfaces);
    }
}

unsigned Scene::RayCount()
{
    return ray_count;
}

void Scene::ResetRayCount()
{
    ray_count = 0;
}
