#ifndef VEC3_H
#define VEC3_H

#include "utils.h"

#include <cmath>

struct Vec3 {
    float x, y, z;
    Vec3() : x(0), y(0), z(0) { }
    Vec3(float x, float y, float z) : x(x), y(y), z(z) { }
    Vec3(float x) : Vec3(x, x, x) { }

    inline float Length() const { return sqrtf(x*x + y*y + z*z); };
    inline float LengthN(float n) const { return pow(pow(x, n) + pow(y, n) + pow(z, n), 1.f/n); };
    inline float LengthSquared() const { return x*x + y*y + z*z; };
    inline Vec3 Normalized() const { float d = Length(); return { x / d, y / d, z / d }; };
    inline float MinComponent() const { return Min(x, Min(y, z)); };
    inline float MaxComponent() const { return Max(x, Max(y, z)); };
};

static inline Vec3 RandomUnitVector() { return Vec3(RandomUniform(-1,1), RandomUniform(-1,1), RandomUniform(-1,1)).Normalized(); }

static inline Vec3 operator*(const Vec3& lhs, const float rhs)                  { return { lhs.x*rhs, lhs.y*rhs, lhs.z*rhs }; }
static inline Vec3 operator/(const Vec3& lhs, const float rhs)                  { return { lhs.x/rhs, lhs.y/rhs, lhs.z/rhs }; }
static inline Vec3 operator+(const Vec3& lhs, const Vec3& rhs)                  { return { lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z }; }
static inline Vec3 operator-(const Vec3& lhs, const Vec3& rhs)                  { return { lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z }; }
static inline Vec3 operator*(const Vec3& lhs, const Vec3& rhs)                  { return { lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z }; }
static inline Vec3 operator/(const Vec3& lhs, const Vec3& rhs)                  { return { lhs.x/rhs.x, lhs.y/rhs.y, lhs.z/rhs.z }; }
static inline Vec3& operator+=(Vec3& lhs, const Vec3& rhs)                      { lhs.x += rhs.x; lhs.y += rhs.y; lhs.z += rhs.z; return lhs; }
static inline Vec3& operator-=(Vec3& lhs, const Vec3& rhs)                      { lhs.x -= rhs.x; lhs.y -= rhs.y; lhs.z -= rhs.z; return lhs; }
static inline Vec3& operator*=(Vec3& lhs, const float rhs)                      { lhs.x *= rhs; lhs.y *= rhs; lhs.z *= rhs; return lhs; }
static inline Vec3& operator/=(Vec3& lhs, const float rhs)                      { lhs.x /= rhs; lhs.y /= rhs; lhs.z /= rhs; return lhs; }
static inline Vec3 operator-(const Vec3& v)                                     { return { -v.x, -v.y, -v.z }; }
static inline bool operator==(const Vec3& lhs, const Vec3& rhs)                 { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;}

static inline float Dot(const Vec3& a, const Vec3& b)       { return a.x*b.x + a.y*b.y + a.z*b.z; }
static inline Vec3 Cross(const Vec3& a, const Vec3& b)      { return { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x }; }

static inline Vec3 Pow(const Vec3& v, const float a)        { return { pow(v.x, a), pow(v.y, a), pow(v.z, a) }; }
static inline Vec3 Floor(const Vec3& v)                     { return { floorf(v.x), floorf(v.y), floorf(v.z) }; }
static inline Vec3 Abs(const Vec3& v)                       { return { fabsf(v.x), fabsf(v.y), fabsf(v.z) }; }

#endif
