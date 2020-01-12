#include "onb.h"

#include "vec3.h"
#include "utils.h"

#include <math.h>

ONB::ONB(const Vec3& normal)
{
    Vec3 n = Normalized(normal), major_axis;
    if(fabs(n.x) < M_SQRT1_3) major_axis = { 1, 0, 0 };
    else if(fabs(n.y) < M_SQRT1_3) major_axis = { 0, 1, 0 };
    else major_axis = { 0, 0, 1 };

    this->u = Normalized(Cross(n, major_axis));
    this->v = Cross(n, u);
    this->w = n;
}

Vec3 ONB::WorldToLocal(const Vec3& p) const
{
    return { Dot(this->u, p), Dot(this->v, p), Dot(this->w, p)};
}

Vec3 ONB::LocalToWorld(const Vec3& p) const
{
    double x = u.x*p.x + v.x*p.y + w.x*p.z;
    double y = u.y*p.x + v.y*p.y + w.y*p.z;
    double z = u.z*p.x + v.z*p.y + w.z*p.z;
    return { x, y, z };
}
