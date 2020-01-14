#include "material.h"

#include "utils.h"
#include "hit.h"

#include <math.h>


///////////////////// STATIC UTILITY FUNCTIONS START ////////////////////////////

// Schlick approximation of fresnel term of a dielectric surface
static inline double Schlick(double cosine, double ref_idx)
{
    double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0*r0;
    return r0 + (1.0 - r0)*pow(1.0 - cosine, 5);
}

// these functions must/should be used for vectors in a local coordinate space w.r.t to a given normal
// this reduces the calculation of dot products down to accessing the z-variable of a vector. 

static inline double AbsCosTheta(const Vec3& w) { return fabs(w.z); }
static inline double SinTheta(const Vec3& w) { return sqrt(Max(0.0, 1.0 - w.z*w.z)); }

static inline double SinPhi(const Vec3& w)
{
    double sintheta = SinTheta(w);
    return sintheta == 0 ? 0.0 : Clamp(w.y / sintheta, -1.0, 1.0);
}

static inline double CosPhi(const Vec3& w)
{
    double sintheta = SinTheta(w);
    return sintheta == 0 ? 1.0 : Clamp(w.x / sintheta, -1.0, 1.0);
}

static inline bool SameHemisphere(const Vec3& wo, const Vec3& wi)
{
    return wo.z*wi.z > 0;
}

///////////////////// STATIC UTILITY FUNCTIONS END ////////////////////////////

Vec3 Lambertian::Eval(const Vec3& wo, const Vec3& wi, const HitRecord& hr) const
{
    return this->albedo->Sample(hr.u, hr.v, hr.position) / M_PI;
}

Vec3 Lambertian::Sample(const Vec3& wo, bool* is_specular) const
{
    Vec3 wi = CosineSampleHemisphere();
    if(wo.z < 0) wi.z *= -1;
    *is_specular = false;
    return wi;
}

double Lambertian::Pdf(const Vec3& wo, const Vec3& wi) const
{
    return SameHemisphere(wo, wi) ? fabs(wi.z) / M_PI : 0.0;
}

Vec3 Specular::Eval(const Vec3& wo, const Vec3& wi, const HitRecord& hr) const
{
    return this->albedo->Sample(hr.u, hr.v, hr.position)*Schlick(fabs(wo.z), this->eta);
}

Vec3 Specular::Sample(const Vec3& wo, bool* is_specular) const
{
    Vec3 wi = { -wo.x, -wo.y, wo.z };
    *is_specular = true;
    return wi;
}

Vec3 DiffuseLight::Sample(const Vec3& wo, bool* is_specular) const
{
    *is_specular = false;
    return Vec3(0);
}

Vec3 DiffuseLight::Emitted(const HitRecord& hr) const
{ 
    return this->emit->Sample(hr.u, hr.v, hr.position);
}

Vec3 Isotropic::Eval(const Vec3& wo, const Vec3& wi, const HitRecord& hr) const
{
    return this->albedo->Sample(hr.u, hr.v, hr.position);
}

Vec3 Isotropic::Sample(const Vec3& wo, bool* is_specular) const
{
    Vec3 wi = Normalized(RandomInUnitSphere());
    *is_specular = true;
    return wi;
}

OrenNayar::OrenNayar(Texture* t, double sigma) : albedo(t)
{
    double sigma_rad = DEG2RAD(sigma);
    double sigma_rad2 = sigma_rad*sigma_rad;
    this->A = 1.0 - (sigma_rad2 / (2.0*(sigma_rad2 + 0.33)));
    this->B = 0.45*sigma_rad2 / (sigma_rad2 + 0.09);
}

Vec3 OrenNayar::Eval(const Vec3& wo, const Vec3& wi, const HitRecord& hr) const
{
    double maxcos = 0.0;
    double sinthetai = SinTheta(wi), sinthetao = SinTheta(wo);
    if(sinthetai > M_EPS && sinthetao > M_EPS) {
        double sinphii = SinPhi(wi), cosphii = CosPhi(wi);
        double sinphio = SinPhi(wo), cosphio = CosPhi(wo);
        maxcos = Max(0.0, cosphii*cosphio + sinphii*sinphio);
    }
    double sinalpha = AbsCosTheta(wi) > AbsCosTheta(wo) ? sinthetao : sinthetai;
    double tanbeta  = AbsCosTheta(wi) > AbsCosTheta(wo) ? sinthetai / AbsCosTheta(wi) : sinthetao / AbsCosTheta(wo);
    return this->albedo->Sample(hr.u, hr.v, hr.position)*(this->A + this->B*maxcos*sinalpha*tanbeta) / M_PI;
}

Vec3 OrenNayar::Sample(const Vec3& wo, bool* is_specular) const
{
    Vec3 wi = CosineSampleHemisphere();
    if(wo.z < 0) wi.z *= -1;
    *is_specular = false;
    return wi;
}

double OrenNayar::Pdf(const Vec3& wo, const Vec3& wi) const
{
    return SameHemisphere(wo, wi) ? fabs(wi.z) / M_PI : 0.0;
}

Vec3 Dielectric::Eval(const Vec3& wo, const Vec3& wi, const HitRecord& hr) const
{
    return this->albedo->Sample(hr.u, hr.v, hr.position);
}

Vec3 Dielectric::Sample(const Vec3& wo, bool* is_specular) const
{
    Vec3 normal = wo.z < 0 ? Vec3(0.0, 0.0, -1.0) : Vec3(0.0, 0.0, 1.0);
    double ratio = wo.z < 0 ? this->eta : 1.0 / this->eta;
    Vec3 refracted;
    double reflect_prob = Refract(-wo, normal, ratio, &refracted) ? Schlick(AbsCosTheta(wo), this->eta) : 1.0;
    *is_specular = true;
    return RandomUniform() < reflect_prob ? Vec3(-wo.x, -wo.y, wo.z) : refracted;
}

Vec3 Velvet::Eval(const Vec3& wo, const Vec3& wi, const HitRecord& hr) const
{
    double costhetao = Max(0.0, wo.z), costhetai = Max(0.0, wi.z);
    double sinthetao = sqrt(1.0 - costhetao*costhetao);
    double horizon_scatter = pow(sinthetao, this->f);
    return horizon_scatter*costhetai*this->albedo->Sample(hr.u, hr.v, hr.position) / M_PI;
}

Vec3 Velvet::Sample(const Vec3& wo, bool* is_specular) const
{
    Vec3 wi = CosineSampleHemisphere();
    if(wo.z < 0) wi.z *= -1;
    *is_specular = false;
    return wi;
}

double Velvet::Pdf(const Vec3& wo, const Vec3& wi) const
{
    return SameHemisphere(wo, wi) ? fabs(wi.z) / M_PI : 0.0;
}
