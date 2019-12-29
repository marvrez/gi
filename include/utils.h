#ifndef UTILS_H
#define UTILS_H

#define INF 1e9
#define EPS 1e-6

// reinventing the wheel so we don't have to import std headers and drastically increase the compile time..
template<typename T> static inline T Min(T lhs, T rhs)          { return lhs < rhs ? lhs : rhs; }
template<typename T> static inline T Max(T lhs, T rhs)          { return lhs >= rhs ? lhs : rhs; }
template<typename T> static inline T Clamp(T v, T mn, T mx)     { return (v < mn) ? mn : (v > mx) ? mx : v; }
template<typename T> static inline T Lerp(T a, T b, float t)    { return (T)(a + (b - a) * t); }
template<typename T> static inline void Swap(T& a, T& b)        { T tmp = a; a = b; b = tmp; }

float RandomUniform(float a=0.f, float b=1.f);
double TimeNow();

#endif
