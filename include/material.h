#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"
#include "texture.h"
#include "microfacet_distribution.h"

#include <memory>

// we assume that the input vectors are given in a local coordinate system w.r.t the hit normal
// this means that cos(theta) = w_in.z, simplifying many of the calculations.

struct HitRecord;

class Material {
public:
    virtual Vec3 Eval(const Vec3& wo, const Vec3& wi, const HitRecord& hr) const { return Vec3(0); }
    virtual Vec3 Sample(const Vec3& wo, bool* is_specular) const { return Vec3(0); }
    virtual double Pdf(const Vec3& wo, const Vec3& wi) const { return 0; }
    virtual Vec3 Emitted(const HitRecord& hr) const { return Vec3(0); }
    virtual bool Emittable() const { return false; }
};

class Lambertian : public Material {
private:
    std::shared_ptr<Texture> albedo;
public:
    Lambertian(Texture* t) : albedo(t) {}
    Lambertian(Vec3 col) : albedo(std::make_shared<SolidTexture>(col)) {}

    virtual Vec3 Eval(const Vec3& wo, const Vec3& wi, const HitRecord& hr) const;
    virtual Vec3 Sample(const Vec3& wo, bool* is_specular) const;
    virtual double Pdf(const Vec3& wo, const Vec3& wi) const;
};

class Specular : public Material {
private:
    std::shared_ptr<Texture> albedo;
    double eta;
public:
    Specular(Texture* t, double eta=0) : albedo(t), eta(eta) {}
    Specular(Vec3 col, double eta=0) : albedo(std::make_shared<SolidTexture>(col)), eta(eta) {}

    virtual Vec3 Eval(const Vec3& wo, const Vec3& wi, const HitRecord& hr) const;
    virtual Vec3 Sample(const Vec3& wo, bool* is_specular) const;
};

class DiffuseLight : public Material {
private:
    std::shared_ptr<Texture> emit;
public:
    DiffuseLight() : emit(std::make_shared<SolidTexture>(Vec3(1.f))) {}
    DiffuseLight(Texture* t) : emit(t) {}
    DiffuseLight(Vec3 col) : emit(std::make_shared<SolidTexture>(col)) {}

    virtual Vec3 Sample(const Vec3& wo, bool* is_specular) const;
    virtual Vec3 Emitted(const HitRecord& hr) const;
    virtual bool Emittable() const { return true; }
};

class Isotropic : public Material {
private:
    std::shared_ptr<Texture> albedo;
public:
    Isotropic(Texture* t) : albedo(t) {}
    Isotropic(Vec3 col) : albedo(std::make_shared<SolidTexture>(col)) {}

    virtual Vec3 Eval(const Vec3& wo, const Vec3& wi, const HitRecord& hr) const;
    virtual Vec3 Sample(const Vec3& wo, bool* is_specular) const;
};

class OrenNayar : public Material {
private:
    std::shared_ptr<Texture> albedo;
    double A, B;
public:
    // sigma in degrees
    OrenNayar(Texture* t, double sigma=20.0);
    OrenNayar(Vec3 col, double sigma=20.0) : OrenNayar(new SolidTexture(col), sigma) {}

    virtual Vec3 Eval(const Vec3& wo, const Vec3& wi, const HitRecord& hr) const;
    virtual Vec3 Sample(const Vec3& wo, bool* is_specular) const;
    virtual double Pdf(const Vec3& wo, const Vec3& wi) const;
};

class Dielectric : public Material {
private:
    std::shared_ptr<Texture> albedo;
    double eta;
public:
    Dielectric(Vec3 col, double eta=1.5) : albedo(new SolidTexture(col)), eta(eta) {}
    Dielectric(double eta=1.5) : Dielectric(Vec3(1.0), eta) {}
    Dielectric(Texture* t, double eta=1.5) : albedo(t), eta(eta) {}

    virtual Vec3 Eval(const Vec3& wo, const Vec3& wi, const HitRecord& hr) const;
    virtual Vec3 Sample(const Vec3& wo, bool* is_specular) const;
};

class Velvet : public Material {
private:
    std::shared_ptr<Texture> albedo;
    double f;
public:
    Velvet(Texture* t, double factor) : albedo(t), f(factor) {}
    Velvet(Vec3 col, double factor) : albedo(std::make_shared<SolidTexture>(col)), f(factor) {}

    virtual Vec3 Eval(const Vec3& wo, const Vec3& wi, const HitRecord& hr) const;
    virtual Vec3 Sample(const Vec3& wo, bool* is_specular) const;
    virtual double Pdf(const Vec3& wo, const Vec3& wi) const;
};

class Microfacet : public Material {
private:
    std::shared_ptr<Texture> albedo;
    std::shared_ptr<MicrofacetDistribution> dist;
    double eta;
public:
    Microfacet(Texture* albedo, MicrofacetDistribution* distribution, double eta)
        : albedo(albedo), dist(distribution), eta(eta) {}
    Microfacet(Vec3 col, MicrofacetDistribution* distribution, double eta)
        : Microfacet(new SolidTexture(col), distribution, eta) {}

    virtual Vec3 Eval(const Vec3& wo, const Vec3& wi, const HitRecord& hr) const;
    virtual double Pdf(const Vec3& wo, const Vec3& wi) const;
    virtual Vec3 Sample(const Vec3& wo, bool* is_specular) const;
};

class FresnelBlend : public Material {
private:
    std::shared_ptr<Texture> rd, rs; // diffuse and specular reflection textures
    std::shared_ptr<MicrofacetDistribution> dist;
public:
    FresnelBlend(Vec3 col_d, Vec3 col_s, MicrofacetDistribution* distribution)
        : FresnelBlend(new SolidTexture(col_d), new SolidTexture(col_s), distribution) {}
    FresnelBlend(Texture* rd, Texture* rs, MicrofacetDistribution* distribution)
        : rd(rd), rs(rs), dist(distribution) {}

    virtual Vec3 Eval(const Vec3& wo, const Vec3& wi, const HitRecord& hr) const;
    virtual double Pdf(const Vec3& wo, const Vec3& wi) const;
    virtual Vec3 Sample(const Vec3& wo, bool* is_specular) const;
};


#endif
