#ifndef HIT_H
#define HIT_H

#include "vec3.h"
#include "material.h"

class Surface;
struct Ray;
struct HitRecord;

struct Hit {
    double t;
    const Surface* s;

    void RecordHit(double t, const Surface* s);
    HitRecord GetRecord(const Ray& r);
};

struct HitRecord {
    double t;
    Vec3 position;
    Vec3 normal;
    double u, v;
    Material material;
};

#endif
