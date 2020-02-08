#ifndef SAMPLER_H
#define SAMPLER_H

class Scene;
struct Ray;
struct Vec3;

Vec3 Sample(Scene* scene, const Ray& ray, int min_bounces=4, int max_bounces=50);
Vec3 SampleAO(Scene* scene, const Ray& ray, int num_samples=1);

#endif
