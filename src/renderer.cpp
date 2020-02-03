#include "renderer.h"

#include "ray.h"
#include "camera.h"
#include "scene.h"
#include "vec3.h"
#include "sampler.h"
#include "loading_bar.h"

#ifdef EMBREE
#include <pmmintrin.h>
#include <xmmintrin.h>
#endif

#include <stdio.h>
#include <string>
#include <thread>

Renderer::Renderer(Scene* scene, Camera* cam, int w, int h, int spp)
    : scene(scene), cam(cam), spp(spp), global_ray_count(0)
{
    this->img = Image(w,h);
    this->num_threads = std::thread::hardware_concurrency();
    this->scene->Build();
}

static inline std::string GetFileExtension(const std::string& filename)
{
    if(filename.find_last_of(".") != std::string::npos)
        return filename.substr(filename.find_last_of(".")+1);
    return "";
}

void Renderer::SaveImage(std::string filename, int iter)
{
    if(filename.find("%") != std::string::npos ) {
        char formatted_filename[256];
        sprintf(formatted_filename, filename.c_str(), iter);
        filename = formatted_filename;
    }
    std::string extension = GetFileExtension(filename);
    if(extension == "png") this->img.SavePNG(filename.c_str());
    else if(extension == "jpg" || extension == "jpeg") this->img.SaveJPG(filename.c_str());
    else if(extension == "ppm") this->img.SavePPM(filename.c_str());
    else {
        printf("Unsupported filetype '%s', must either be png, jpg or ppm. Exiting program...\n", extension.c_str());
        exit(0);
    }
}

void Renderer::RenderFrame(int thread_id, LoadingBar* lb)
{
    Scene::ResetRayCount();
    int w = this->img.Width(), h = this->img.Height();
    for(int y = thread_id; y < h; y += this->num_threads) {
        for(int x = 0; x < w; ++x) {
            for(int s = 0; s < spp; ++s) {
                double u = (x + RandomUniform()) / (double)w;
                double v = (y + RandomUniform()) / (double)h;
                Ray ray = this->cam->CastRay(u, 1.0-v);
                Rgb col = Sample(this->scene, ray);
                this->img.AddPixel(x, y, col);
            }
        }
        if(lb != nullptr) lb->Update();
    }
    this->global_ray_count += Scene::RayCount();
}

void Renderer::Render(std::string filename, int num_iterations)
{
#ifdef EMBREE
    // Intel says to do this, so we're doing it.
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
#endif
    for(int iter = 1; iter <= num_iterations; ++iter) {
        printf("Iteration %d\n", iter);
        LoadingBar lb(this->img.Height());
        this->global_ray_count = 0;

        double t1 = TimeNow();
        std::vector<std::thread> threads;
        for(int tid = 0; tid < this->num_threads; ++tid) {
            threads.emplace_back(&Renderer::RenderFrame, this, tid, &lb);
        }
        for(auto& t : threads) t.join();
        double t2 = TimeNow();

        char end_msg[64];
        sprintf(end_msg, "[%3.2f fps, %3.2f Mray/s]", 1./(t2-t1), double(this->global_ray_count.load())/(1e6*(t2-t1)));
        lb.Done(end_msg);

        this->SaveImage(filename, iter);
    }
}
