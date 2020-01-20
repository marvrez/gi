#include "utils.h"

#include "vec3.h"

#include <chrono>
#include <random>
#include <math.h>

static thread_local std::random_device rd;
static thread_local std::mt19937 rng(rd());

double RandomUniform(double a, double b)
{
    std::uniform_real_distribution<double> uniform_dist(a, b);
    return uniform_dist(rng);
}

int RandomUniform(int a, int b)
{
    std::uniform_int_distribution<int> uniform_dist(a, b);
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

Vec3 HexColor(int hex)
{
    double r = double((hex >> 16) & 0xff) / 255;
    double g = double((hex >>  8) & 0xff) / 255;
    double b = double((hex >>  0) & 0xff) / 255;
    return Pow(Vec3(r, g, b), 2.2);
}

static inline double Func(double a, double b, double c, double x) { return a + b*x + c*log(x); }
// http://www.zombieprototypes.com/?p=210
Vec3 ColorTemperature(double K)
{
    double red, green, blue;

    // red
    if(K >= 6600) red = Func(351.97690566805693, 0.114206453784165, -40.25366309332127, K/100 - 55);
    else red = 255;

    // green
    if(K >= 6600) green = Func(325.4494125711974, 0.07943456536662342, -28.0852963507957, K/100 - 50);
    else if(K >= 1000) green = Func(-155.25485562709179, -0.44596950469579133, 104.49216199393888, K/100 - 2);
    else green = 0;

    // blue
    if(K >= 6600) blue = 255;
    else if(K >= 2000) blue = Func(-254.76935184120902, 0.8274096064007395, 115.67994401066147, K/100 - 10);
    else blue = 0;

    red = Min(1.0, red / 255), green = Min(1.0, green / 255), blue = Min(1.0, blue / 255);
    return Vec3(red, green, blue);
}

double TimeNow()
{
    using namespace std::chrono;
    nanoseconds now = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());
    return now.count()*1e-9;
}
