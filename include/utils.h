#ifndef UTILS_H
#define UTILS_H

constexpr double M_INF      = 1e9;
constexpr double M_EPS      = 1e-6;
constexpr double M_SQRT1_3  = 0.57735026918962576450914878050195746;
#ifndef M_PI
constexpr double M_PI = 3.14159265358979323846264338327950288;
#endif
#ifndef M_PI_2
constexpr double M_PI_2 = 1.57079632679489661923132169163975144;
#endif

#define RAD2DEG(rads) (rads*180./M_PI)
#define DEG2RAD(degs) (degs*M_PI/180.)

enum class Axis : int { NONE = -1, X, Y, Z, };

// reinventing the wheel so we don't have to import std headers and drastically increase the compile time..
template<typename T> static inline T Min(T lhs, T rhs)          { return lhs < rhs ? lhs : rhs; }
template<typename T> static inline T Max(T lhs, T rhs)          { return lhs >= rhs ? lhs : rhs; }
template<typename T> static inline T Clamp(T v, T mn, T mx)     { return (v < mn) ? mn : (v > mx) ? mx : v; }
template<typename T> static inline T Lerp(T a, T b, double t)   { return (T)(a + (b - a)*t); }
template<typename T> static inline void Swap(T& a, T& b)        { T tmp = a; a = b; b = tmp; }

double RandomUniform(double a=0., double b=1.);
int RandomUniform(int a, int b);

struct Vec3;
Vec3 RandomInUnitDisk();
Vec3 RandomInUnitSphere();
Vec3 CosineSampleHemisphere();

double TimeNow();

#endif
