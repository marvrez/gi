#ifndef SCENE_H
#define SCENE_H

#include "kdtree.h"

#include <vector>
#include <memory>

class Surface;
class Texture;
struct Hit;
struct Ray;

class Scene {
private:
    std::vector<Surface*> surfaces;
    std::vector<Surface*> lights;
    std::unique_ptr<KDTree> tree;

    std::shared_ptr<Texture> background_texture;
    Vec3 background_color;

public:
    Scene() : background_texture(nullptr), background_color(Vec3(0.0)) {};
    void Add(Surface* s);
    void Add(std::shared_ptr<Surface> s);
    bool Intersect(const Ray& r, Hit* h);
    void Build();
    std::vector<Surface*> Lights() const { return this->lights; }

    void SetBackgroundColor(const Vec3& col) { this->background_color = col; }
    void SetBackgroundTexture(const std::shared_ptr<Texture>& t) { this->background_texture = t; }
    Vec3 BackgroundColor() const { return this->background_color; }
    Texture* BackgroundTexture() const { return this->background_texture.get(); }

    static unsigned RayCount();
    static void ResetRayCount();
};

#endif
