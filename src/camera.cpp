#include "camera.h"

#include "ray.h"
#include "utils.h"

Camera::Camera(const Vec3& lookfrom, const Vec3& lookat, const Vec3& up,
        double vfov, double aspect, double aperture, double focus_dist)
{
    double d = focus_dist;
    double theta = DEG2RAD(vfov);
    double half_height = tan(theta / 2.), half_width = aspect*half_height;

    this->w = (lookfrom - lookat).Normalized();
    this->u = Cross(up, this->w).Normalized();
    this->v = Cross(this->w, this->u).Normalized();

    this->aperture_radius = aperture / 2.;

    this->origin        = lookfrom;
    this->lower_left    = lookfrom - half_width*d*this->u - half_height*d*this->v - d*this->w;

    this->width         = 2.*half_width*d*this->u;
    this->height        = 2.*half_height*d*this->v;
}

Camera::Camera(const Vec3& lookfrom, const Vec3& lookat, const Vec3& up,
        double vfov, double aspect, double aperture) 
    : Camera(lookfrom, lookat, up, vfov, aspect, aperture, (lookat-lookfrom).Length())
{
}

Ray Camera::CastRay(double u, double v)
{
    Vec3 rd = RandomInUnitDisk()*this->aperture_radius;
    Vec3 offset = this->u*rd.x + this->v*rd.y;
    Vec3 dir = (this->lower_left + this->width*u + this->height*v - this->origin - offset).Normalized();
    return { this->origin + offset, dir };
}
