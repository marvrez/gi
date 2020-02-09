#include "sampler.h"

#include "scene.h"
#include "ray.h"
#include "vec3.h"
#include "hit.h"
#include "onb.h"
#include "surface.h"

#include <vector>

Vec3 SampleBackground(Scene* scene, const Ray& ray)
{
    Texture* t = scene->BackgroundTexture();
    if(t != nullptr) {
        Vec3 dir = ray.direction;
        double u = atan2(dir.z, dir.x), v = atan2(dir.y, Vec3(dir.x, 0.0, dir.z).Length());
        u = (u + M_PI) / (2*M_PI),      v = (v + M_PI_2) / M_PI;
        return t->Sample(u, v, {});
    }
    return scene->BackgroundColor();
}

Vec3 SampleDirectLighting(Scene* scene, const Surface* light, const ONB& onb, const HitRecord& hr, const Vec3& wo)
{
    Ray light_ray = light->RandomRay(hr.position);
    Hit hit;
    if(scene->Intersect(light_ray, &hit)) {
        HitRecord lhr = hit.GetRecord(light_ray);
        Vec3 li = lhr.material->Emitted(lhr);
        if(li.MaxComponent() > 0 && Dot(lhr.normal, light_ray.direction) < 0) {
            double light_pdf = light->Pdf(light_ray);
            Vec3 lwi = onb.WorldToLocal(light_ray.direction);
            return hr.material->Eval(wo, lwi, hr)*li*fabs(lwi.z) / light_pdf;
        }
    }
    return Vec3(0.0);
}

Vec3 SampleOneLight(Scene* scene, const ONB& onb, const HitRecord& hr, const Vec3& wo)
{
    const auto& lights = scene->Lights();
    int num_lights = lights.size();
    if(num_lights == 0) return Vec3(0.0); // return black if there are no lights
    int idx = RandomUniform(0, num_lights-1);
    const auto& light = lights[idx];
    return num_lights*SampleDirectLighting(scene, light, onb, hr, wo);
}

Vec3 Sample(Scene* scene, const Ray& ray, int min_bounces, int max_bounces)
{
    Vec3 col(0.0), throughput(1.0);
    bool is_specular = true;
    Ray cur_ray = ray;
    for(int num_bounces = 0; num_bounces < max_bounces; ++num_bounces) {
        Hit hit;
        if(!scene->Intersect(cur_ray, &hit)) {
            col += throughput*SampleBackground(scene, cur_ray);
            break;
        }
        HitRecord hr = hit.GetRecord(cur_ray);

        Vec3 emitted = hr.material->Emitted(hr);
        if(emitted.MaxComponent() > 0) {
            if(is_specular && Dot(hr.normal, cur_ray.direction) < 0) {
                col += throughput*emitted;
            }
            break;
        }

        ONB onb(hr.normal);
        Vec3 wo = onb.WorldToLocal(Normalized(-cur_ray.direction));
        Vec3 wi = hr.material->Sample(wo, &is_specular);
        double pdf = hr.material->Pdf(wo, wi);
        Vec3 attenuation = hr.material->Eval(wo, wi, hr);

        if(!is_specular) {
            col += throughput*SampleOneLight(scene, onb, hr, wo);
            if(pdf < M_EPS) break;
            throughput = throughput*attenuation*fabs(wi.z) / pdf;
        }
        else throughput = throughput*attenuation;

        cur_ray = Ray(hr.position, onb.LocalToWorld(wi));

        // russian roulette
        if(num_bounces >= min_bounces) {
            double prob = throughput.MaxComponent();
            if(RandomUniform() > prob) break;
            throughput /= prob;
        }
    }
    return col;
}

Vec3 SampleAO(Scene* scene, const Ray& ray, int num_samples)
{
    Hit hit;
    if(!scene->Intersect(ray, &hit)) return SampleBackground(scene, ray);
    HitRecord hr = hit.GetRecord(ray);

    ONB onb(hr.normal);
    double occlusion = 0;
    for(int i = 0; i < num_samples; ++i) {
        Vec3 wi = CosineSampleHemisphere();
        bool is_visible = scene->Intersect(Ray(hr.position, onb.LocalToWorld(wi)), &hit);
        if(!is_visible) occlusion += 1;
    }
    return Vec3(occlusion / num_samples);
}
