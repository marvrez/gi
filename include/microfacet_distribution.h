#ifndef MICROFACET_H
#define MICROFACET_H

struct Vec3;

class MicrofacetDistribution {
public:
    virtual double Eval(const Vec3& wh) const = 0;
    virtual double Pdf(const Vec3& wo, const Vec3& wi) const = 0;
    virtual Vec3 Sample(const Vec3& wo) const = 0;
    virtual ~MicrofacetDistribution() {}
};

class PowerCosineDistribution : public MicrofacetDistribution {
private:
    double n; // glossiness with range [0, inf), where 0 is a diffuse surface
    double norm1; // normalization constant for calculating the pdf for sampling
    double norm2; // normalization constant for calculating the distribution
public:
    PowerCosineDistribution(double exponent);
    virtual double Eval(const Vec3& wh) const;
    virtual double Pdf(const Vec3& wo, const Vec3& wi) const;
    virtual Vec3 Sample(const Vec3& wo) const;
};

#endif
