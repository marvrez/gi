#ifndef VEC3_H
#define VEC3_H

#include "utils.h"

#include <math.h>

typedef Vec3 Rgb;

struct Vec3 {
    union {
        struct { double x, y, z; };
        struct { double u, v, w; };
        struct { double r, g, b; };
        double data[3];
    };
    Vec3() : x(0), y(0), z(0) { }
    Vec3(double x, double y, double z) : x(x), y(y), z(z) { }
    Vec3(double x) : Vec3(x, x, x) { }

    inline double Length() const            { return sqrt(x*x + y*y + z*z); };
    inline double LengthN(double n) const   { return pow(pow(x, n) + pow(y, n) + pow(z, n), 1./n); };
    inline double LengthSquared() const     { return x*x + y*y + z*z; };
    inline Vec3 Normalized() const          { double d = Length(); return { x / d, y / d, z / d }; };
    inline double MinComponent() const      { return Min(x, Min(y, z)); };
    inline double MaxComponent() const      { return Max(x, Max(y, z)); };

    inline double operator[](int i) const   { return data[i]; }
};

static inline Vec3 RandomUnitVector() { return Vec3(RandomUniform(-1,1), RandomUniform(-1,1), RandomUniform(-1,1)).Normalized(); }

static inline Vec3 operator*(const Vec3& lhs, const double rhs)     { return { lhs.x*rhs, lhs.y*rhs, lhs.z*rhs }; }
static inline Vec3 operator/(const Vec3& lhs, const double rhs)     { return { lhs.x/rhs, lhs.y/rhs, lhs.z/rhs }; }
static inline Vec3 operator+(const Vec3& lhs, const Vec3& rhs)      { return { lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z }; }
static inline Vec3 operator-(const Vec3& lhs, const Vec3& rhs)      { return { lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z }; }
static inline Vec3 operator*(const Vec3& lhs, const Vec3& rhs)      { return { lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z }; }
static inline Vec3 operator/(const Vec3& lhs, const Vec3& rhs)      { return { lhs.x/rhs.x, lhs.y/rhs.y, lhs.z/rhs.z }; }
static inline Vec3& operator+=(Vec3& lhs, const Vec3& rhs)          { lhs.x += rhs.x; lhs.y += rhs.y; lhs.z += rhs.z; return lhs; }
static inline Vec3& operator-=(Vec3& lhs, const Vec3& rhs)          { lhs.x -= rhs.x; lhs.y -= rhs.y; lhs.z -= rhs.z; return lhs; }
static inline Vec3& operator*=(Vec3& lhs, const double rhs)         { lhs.x *= rhs; lhs.y *= rhs; lhs.z *= rhs; return lhs; }
static inline Vec3& operator/=(Vec3& lhs, const double rhs)         { lhs.x /= rhs; lhs.y /= rhs; lhs.z /= rhs; return lhs; }
static inline Vec3 operator-(const Vec3& v)                         { return { -v.x, -v.y, -v.z }; }
static inline bool operator==(const Vec3& lhs, const Vec3& rhs)     { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }

static inline double Dot(const Vec3& a, const Vec3& b)  { return a.x*b.x + a.y*b.y + a.z*b.z; }
static inline Vec3 Cross(const Vec3& a, const Vec3& b)  { return { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x }; }
static inline Vec3 Normalized(const Vec3& a)            { return a.Normalized(); }

static inline Vec3 Pow(const Vec3& v, const double a)   { return { pow(v.x, a), pow(v.y, a), pow(v.z, a) }; }
static inline Vec3 Floor(const Vec3& v)                 { return { floor(v.x), floor(v.y), floor(v.z) }; }
static inline Vec3 Abs(const Vec3& v)                   { return { fabs(v.x), fabs(v.y), fabs(v.z) }; }
static inline Vec3 Fract(const Vec3& v)                 { return { fmod(v.x, 1.), fmod(v.y, 1.), fmod(v.z, 1.) }; }

template<> inline Vec3 Min(Vec3 a, Vec3 b) { return { Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z) }; }
template<> inline Vec3 Max(Vec3 a, Vec3 b) { return { Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z) }; }

// linear mapping from t in [0, 1] to [low, high]
static inline double Mix(double low, double high, double t)                             { return low + (high - low)*t; }
static inline Vec3 Mix(const Vec3& low, const Vec3& high, double t)                     { return low + (high - low)*t; }
static inline Vec3 Mix(const Vec3& low, const Vec3& high, const Vec3& t)                { return low + (high - low)*t; }

static inline bool All(const Vec3& v) { return v.x == 0.0 && v.y == 0.0 && v.z == 0.0; }
static inline bool Any(const Vec3& v) { return v.x == 0.0 || v.y == 0.0 || v.z == 0.0; }

static inline Vec3 Reflect(const Vec3& v, const Vec3& n) { return 2.0*Dot(v, n)*n - v; }
static inline bool Refract(const Vec3& v, const Vec3& n, double ratio, Vec3* refracted)
{
    Vec3 uv = Normalized(v);
    double dt = Dot(uv, n), discriminant = 1.0 - ratio*ratio*(1.0 - dt*dt);
    if(discriminant <= 0) return false;
    *refracted = ratio*(uv - n*dt) - n*sqrt(discriminant);
    return true;
}

#endif
