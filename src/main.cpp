#include <cstdio>

#include "gi.h"

constexpr int w = 500, h = 500, num_samples = 16;
constexpr double aspect = (double)w / (double)h;

int main(int argc, char** argv)
{
    Camera cam({3,-3,3}, {0,0,0}, {0,0,1}, 45.0, aspect, 0.01);

    Material* floor_material = new Lambertian(new CheckeredTexture());
    //Material* floor_material = new OrenNayar(Vec3(0.9), 20);

    //PowerCosineDistribution* pcd = new PowerCosineDistribution(100);
    //Material* sphere_material = new Lambertian(HexColor(0x808080));
    Material* sphere_material = new Dielectric({0.4, 0.6, 0.8});
    //Material* sphere_material = new Microfacet(HexColor(0xFFF0A5), pcd, 2.0);
    //Material* sphere_material = new FresnelBlend(HexColor(0xFFF0A5), HexColor(0xFFF0A5), pcd);
    //Material* sphere_material = new Specular(HexColor(0x808080));

    Scene scene;
    scene.Add(new Sphere({0,0,1}, 1,  sphere_material));
    //auto mesh = ReadSTL("dragon.stl", sphere_material);
    //auto mesh = ReadSTL("minified.stl", sphere_material);
    //mesh->Rotate({0,0,1}, DEG2RAD(30));
    //mesh->MoveTo({0, 1, 0.5});
    //mesh->FitInsideUnitCube();
    //mesh->MoveTo({0,0,1});
    //scene.Add(mesh);
    scene.Add(new Sphere({0,0,-999}, 999, floor_material));;
    scene.Add(new Sphere({3,1,4}, 2, new DiffuseLight(ColorTemperature(5000)*7)));
    scene.Build();

    Renderer renderer(&scene, &cam, w, h, num_samples);

    double t1 = TimeNow();
    renderer.Render("test.jpg", 1000);
    double t2 = TimeNow();
    printf("rendering took %f seconds\n", t2-t1);
    return 0;
}
