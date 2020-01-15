#include "microfacet_distribution.h"

#include "utils.h"
#include "vec3.h"

#include <math.h>

PowerCosineDistribution::PowerCosineDistribution(double exponent)
    : n(exponent)
{
    this->norm1 = (exponent+1.0) / (2*M_PI);
    this->norm2 = (exponent+2.0) / M_PI;
}

double PowerCosineDistribution::Eval(const Vec3& wh) const
{
    return this->norm2*pow(fabs(wh.z), this->n);
}

double PowerCosineDistribution::Pdf(const Vec3& wo, const Vec3& wi) const
{
    Vec3 wh = Normalized(wo + wi);
    if(Dot(wo, wh) <= 0) return 0;
    return (this->norm1*pow(fabs(wh.z), this->n)) / (4.0*Dot(wo, wh));
}

Vec3 PowerCosineDistribution::Sample(const Vec3& wo) const
{
    double costheta = pow(RandomUniform(), 1.0 / (this->n + 1.0));
    double sintheta = sqrt(Max(0.0, 1.0 - costheta*costheta));
    double phi = 2.0*M_PI*RandomUniform();
    Vec3 wh = { sintheta*cos(phi), sintheta*sin(phi), costheta }; // spherical direction
    if(wh.z*wo.z < 0) wh = -wh; // in case they are not in the same hemisphere
    return Normalized(Reflect(wo, wh));
}
