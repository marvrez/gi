#include "utils.h"

#include "vec3.h"

#include <chrono>
#include <random>

static std::random_device rd;
static std::mt19937 rng(rd());

double RandomUniform(double a, double b)
{
    std::uniform_real_distribution<double> uniform_dist(a, b);
    return uniform_dist(rng);
}

Vec3 RandomInUnitDisk()
{
    Vec3 v;
    do {
        v = 2.0*Vec3(RandomUniform(), RandomUniform(), 0) - Vec3(1,1,0);
    } while(v.LengthSquared() >= 1.0);
    return v;
}

Vec3 RandomInUnitSphere()
{
    Vec3 v;
    do {
        v = 2.0*Vec3(RandomUniform(), RandomUniform(), RandomUniform()) - Vec3(1, 1, 1);
    } while(v.LengthSquared() >= 1.0);
    return v;
}

double TimeNow()
{
    using namespace std::chrono;
    nanoseconds now = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());
    return now.count()*1e-9;
}
