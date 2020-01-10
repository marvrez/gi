#include <cstdio>
#include <iostream>

#include "utils.h"
#include "vec3.h"
#include "image.h"

#include "sphere.h"
#include "scene.h"
#include "ray.h"
#include "hit.h"
#include "camera.h"

constexpr int w = 500, h = 500, num_samples = 16;
constexpr double aspect = (double)w / (double)h;

int main(int argc, char** argv)
{
    Scene scene;
    scene.Add(new Sphere({0,0,-1}, 0.5, {}));
    scene.Add(new Sphere({0,-100.5,-1}, 100, {}));
    scene.Build();

    Camera cam({0,0,3.3}, {0,0,0}, {0,1,0}, 45.0, aspect);
    Image img(w, h);

    double t1 = TimeNow();
    #pragma omp parallel for
    for(int y = 0; y < h; ++y) {
        Hit hit;
        for(int x = 0; x < w; ++x) {
            for(int s = 0; s < num_samples; ++s) {
                double u = (x + RandomUniform()) / (double)w;
                double v = 1.0 - (y + RandomUniform()) / (double)h;
                Ray r = cam.CastRay(u,v);
                bool did_hit = scene.Intersect(r, &hit);
                HitRecord hr = did_hit ? hit.GetRecord(r) : HitRecord();
                Rgb col = did_hit ? 0.5*(hr.normal+1) : Vec3(0.4f, .5f, .3f);
                img.AddPixel(x, y, col);
            }
        }
    }
    double t2 = TimeNow();
    std::cout << "took " << t2 - t1 << " seconds\n";
    img.SavePNG("test.png");
    return 0;
}
