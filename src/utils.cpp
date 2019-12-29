#include "utils.h"

#include <chrono>
#include <random>

static std::random_device rd;
static std::mt19937 rng(rd());

float RandomUniform(float a, float b)
{
    std::uniform_real_distribution<float> uniform_dist(a, b);
    return uniform_dist(rng);
}

double TimeNow()
{
    using namespace std::chrono;
    nanoseconds now = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());
    return now.count()*1e-9;
}
