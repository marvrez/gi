#include "utils.h"

#include "vec3.h"

#include <chrono>
#include <random>

static thread_local std::random_device rd;
static thread_local std::mt19937 rng(rd());

double RandomUniform(double a, double b)
{
    std::uniform_real_distribution<double> uniform_dist(a, b);
    return uniform_dist(rng);
}

Vec3 RandomInUnitDisk()
{
    double r = sqrt(RandomUniform());
    double theta = RandomUniform()*2.0*M_PI;
    return { r*cos(theta), r*sin(theta), 0.0 };
}

Vec3 RandomInUnitSphere()
{
    double cos_phi = 2.0*RandomUniform() - 1.0;
    double sin_phi = sqrt(Max(0.0, 1.0 - cos_phi*cos_phi));
    double theta = 2*M_PI*RandomUniform();
    return { sin_phi*sin(theta), cos_phi, sin_phi*cos(theta)};
}

Vec3 CosineSampleHemisphere()
{
    Vec3 d = RandomInUnitDisk();
    double z = sqrt(Max(0.0, 1.0 - d.x*d.x - d.y*d.y)); // project z up to the unit hemisphere
    return { d.x, d.y, z };
}

double TimeNow()
{
    using namespace std::chrono;
    nanoseconds now = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());
    return now.count()*1e-9;
}
