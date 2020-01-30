#ifndef VEC3_H
#define VEC3_H

#include "utils.h"

#include <math.h>
#include <functional>

typedef Vec3 Rgb;

#ifdef __AVX__

#include <immintrin.h>

static double Dot(const Vec3& a, const Vec3& b);
struct Vec3 {
    union {
        struct { double x, y, z, pad1; };
        struct { double u, v, w, pad2; };
        struct { double r, g, b, pad3; };
        double data[4];
        __m256d d256;
    };
    Vec3() : x(0), y(0), z(0), pad1(0) { }
    Vec3(double x, double y, double z) : x(x), y(y), z(z), pad1(0) { }
    explicit Vec3(const __m256d& _avx) : d256(_avx) { }
    Vec3(double x) : d256(_mm256_set1_pd(x)) {}

    inline double Length() const            { return sqrt(LengthSquared()); };
    inline double LengthN(double n) const   { return pow(pow(x, n) + pow(y, n) + pow(z, n), 1./n); };
    inline double LengthSquared() const     { return Dot(*this, *this); };
    inline Vec3 Normalized() const          { return *this / Length(); };
    inline double MinComponent() const      { return Min(x, Min(y, z)); };
    inline double MaxComponent() const      { return Max(x, Max(y, z)); };

    inline double operator[](int i) const   { return data[i]; }

    inline Vec3 operator+(const Vec3 other) const { return Vec3(_mm256_add_pd(d256, other.d256)); }
    inline Vec3 operator-(const Vec3 other) const { return Vec3(_mm256_sub_pd(d256, other.d256)); }
    inline Vec3 operator*(const Vec3 other) const { return Vec3(_mm256_mul_pd(d256, other.d256)); }
    inline Vec3 operator/(const Vec3 other) const { return Vec3(_mm256_div_pd(d256, other.d256)); }

    inline Vec3& operator+=(const Vec3& v) { d256 = _mm256_add_pd(d256, v.d256); return *this; }
    inline Vec3& operator-=(const Vec3& v) { d256 = _mm256_sub_pd(d256, v.d256); return *this; }

    inline Vec3 operator*(const double rhs) const { return Vec3(_mm256_mul_pd(d256, _mm256_set1_pd(rhs))); }
    inline Vec3 operator/(const double rhs) const { return Vec3(_mm256_div_pd(d256, _mm256_set1_pd(rhs))); }

    inline Vec3& operator/=(const double rhs) { d256 = _mm256_div_pd(d256, _mm256_set1_pd(rhs)); return *this; }
    inline Vec3& operator*=(const double rhs) { d256 = _mm256_mul_pd(d256, _mm256_set1_pd(rhs)); return *this; }

    inline Vec3 operator-() const { return Vec3(_mm256_xor_pd(d256, _mm256_castsi256_pd(_mm256_set1_epi32(0x80000000)))); }
};

static inline Vec3 operator*(const double lhs, const Vec3& rhs) { return Vec3(_mm256_mul_pd(rhs.d256, _mm256_set1_pd(lhs))); }

static inline double Dot(const Vec3& a, const Vec3& b)
{
    __m256d ab = _mm256_mul_pd(a.d256, b.d256); // [x1*x2, y1*y2, z1*z2, 0]

    __m128d ablow  = _mm256_castpd256_pd128(ab); // [z1*z2, 0]
    __m128d abhigh = _mm256_extractf128_pd(ab, 1); // [x1*x2, y1*y2]
    __m128d sum1 =   _mm_add_pd(ablow, abhigh); // [z1*z2 + x1*x2, y1*y2]

    __m128d swapped = _mm_shuffle_pd(sum1, sum1, 0b01); // // [y1*y2, x1*x2 + z1*z2]
    __m128d dotproduct = _mm_add_pd(sum1, swapped);
    return _mm_cvtsd_f64(dotproduct);
}

static inline Vec3 Cross(const Vec3& a, const Vec3& b)
{
    return Vec3(_mm256_sub_pd(
               _mm256_mul_pd(
                     _mm256_permute4x64_pd(a.d256, _MM_SHUFFLE(3, 0, 2, 1)),
                     _mm256_permute4x64_pd(b.d256, _MM_SHUFFLE(3, 1, 0, 2))),
               _mm256_mul_pd(
                     _mm256_permute4x64_pd(a.d256, _MM_SHUFFLE(3, 1, 0, 2)),
                     _mm256_permute4x64_pd(b.d256, _MM_SHUFFLE(3, 0, 2, 1)))
            ));;
}

template<> inline Vec3 Min(Vec3 a, Vec3 b) { return Vec3(_mm256_min_pd(a.d256, b.d256)); }
template<> inline Vec3 Max(Vec3 a, Vec3 b) { return Vec3(_mm256_max_pd(a.d256, b.d256)); }

#else

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

static inline double Dot(const Vec3& a, const Vec3& b)  { return a.x*b.x + a.y*b.y + a.z*b.z; }
static inline Vec3 Cross(const Vec3& a, const Vec3& b)  { return { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x }; }

template<> inline Vec3 Min(Vec3 a, Vec3 b) { return { Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z) }; }
template<> inline Vec3 Max(Vec3 a, Vec3 b) { return { Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z) }; }

#endif

static inline bool operator==(const Vec3& lhs, const Vec3& rhs)     { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }

static inline Vec3 Normalized(const Vec3& a)            { return a.Normalized(); }
static inline Vec3 RandomUnitVector() { return Vec3(RandomUniform(-1,1), RandomUniform(-1,1), RandomUniform(-1,1)).Normalized(); }

static inline Vec3 Pow(const Vec3& v, const double a)   { return { pow(v.x, a), pow(v.y, a), pow(v.z, a) }; }
static inline Vec3 Floor(const Vec3& v)                 { return { floor(v.x), floor(v.y), floor(v.z) }; }
static inline Vec3 Abs(const Vec3& v)                   { return { fabs(v.x), fabs(v.y), fabs(v.z) }; }
static inline Vec3 Fract(const Vec3& v)                 { return { fmod(v.x, 1.), fmod(v.y, 1.), fmod(v.z, 1.) }; }

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

static inline void hash_combine(size_t& seed, size_t hash) { hash += 0x9e3779b9 + (seed << 6) + (seed >> 2); seed ^= hash; }
namespace std
{
    template<> struct hash<Vec3> {
        size_t operator()(Vec3 const& v) const
        {
            size_t seed = 0;
            hash<double> hasher;
            hash_combine(seed, hasher(v.x));
            hash_combine(seed, hasher(v.y));
            hash_combine(seed, hasher(v.z));
            return seed;
        }
    };
}

#endif
