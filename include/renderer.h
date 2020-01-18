#ifndef RENDERER_H
#define RENDERER_H

#include "image.h"

#include <string>
#include <atomic>

class Scene;
class Camera;
class LoadingBar;

class Renderer {
private:
    Scene* scene;
    Camera* cam;
    Image img;
    int spp;
    int num_threads;
    std::atomic<int> global_ray_count;

    void SaveImage(std::string filename, int iter);
    void RenderFrame(int thread_id, LoadingBar* lb);

public:
    Renderer(Scene* scene, Camera* cam, int w, int h, int spp=16);
    void Render(std::string filename, int num_iterations=M_INF);
};

#endif
