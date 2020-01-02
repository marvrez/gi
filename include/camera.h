#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"

struct Ray;

class Camera {
  private:
    Vec3 origin;
    Vec3 lower_left;
    Vec3 width, height;
    Vec3 u, v, w; // basis

    double aperture_radius;

public:
    Camera(const Vec3& lookfrom, const Vec3& lookat, const Vec3& up,
            double vfov, double aspect, double aperture, double focus_dist);
    Camera(const Vec3& lookfrom, const Vec3& lookat, const Vec3& up,
            double vfov, double aspect);
    Ray CastRay(double u, double v);
};

#endif
