#include <cstdio>

#include "utils.h"
#include "sphere.h"
#include "scene.h"
#include "camera.h"
#include "renderer.h"
#include "microfacet_distribution.h"

constexpr int w = 500, h = 500, num_samples = 100;
constexpr double aspect = (double)w / (double)h;

inline Vec3 HexColor(const int hex) {
    double r = double((hex >> 16) & 0xff) / 255;
    double g = double((hex >> 8) & 0xff) / 255;
    double b = double((hex >> 0) & 0xff) / 255;
    return Pow(Vec3(r, g, b), 2.2);
}

int main(int argc, char** argv)
{
    Camera cam({0,5,3}, {0,0,0}, {0,0,1}, 45.0, aspect, 0.01);

    //Material* floor_material = new OrenNayar(Vec3(0.9), 20);
    //Material* floor_material = new Lambertian(new CheckeredTexture());
    Material* floor_material = new OrenNayar(Vec3(0.9), 20);

    PowerCosineDistribution* pcd = new PowerCosineDistribution(100);
    //Material* sphere_material = new Dielectric();//{0.4, 0.6, 0.8});
    //Material* sphere_material = new Microfacet(HexColor(0xFFF0A5), pcd, 2.0);
    Material* sphere_material = new FresnelBlend(HexColor(0xFFF0A5), HexColor(0xFFF0A5), pcd);
    //Material* sphere_material = new Specular(HexColor(0x808080));

    Scene scene;
    scene.Add(new Sphere({0,0,1}, 1,  sphere_material));
    scene.Add(new Sphere({0,0,-999}, 999, floor_material));;
    scene.Add(new Sphere({3,1,4}, 1, new DiffuseLight({7.5, 6.70143, 6.03107})));
    scene.Build();

    Renderer renderer(&scene, &cam, w, h, num_samples);

    double t1 = TimeNow();
    renderer.Render("test.jpg", 1000);
    double t2 = TimeNow();
    printf("rendering took %f seconds\n", t2-t1);
    return 0;
}
