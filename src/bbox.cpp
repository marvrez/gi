#include "bbox.h"

#include "ray.h"
#include "surface.h"
#include "utils.h"

BBox BBox::Union(const BBox& b) const
{
    return { Min(this->min_point, b.min_point), Max(this->max_point, b.max_point) };
}

bool BBox::Contains(const Vec3& p) const
{
    return this->min_point.x <= p.x && this->max_point.x >= p.x &&
           this->min_point.y <= p.y && this->max_point.y >= p.y &&
           this->min_point.z <= p.z && this->max_point.z >= p.z;
}

bool BBox::Intersects(const BBox& b)  const
{
    return !(this->min_point.x > b.max_point.x || this->max_point.x < b.min_point.x || this->min_point.y > b.max_point.y ||
             this->max_point.y < b.min_point.y || this->min_point.z > b.max_point.z || this->max_point.z < b.min_point.z);
}

bool BBox::Intersect(const Ray& r, double* tmin, double* tmax) const
{
    double x1 = (this->min_point.x - r.origin.x) / r.direction.x;
    double y1 = (this->min_point.y - r.origin.y) / r.direction.y;
    double z1 = (this->min_point.z - r.origin.z) / r.direction.z;
    double x2 = (this->max_point.x - r.origin.x) / r.direction.x;
    double y2 = (this->max_point.y - r.origin.y) / r.direction.y;
    double z2 = (this->max_point.z - r.origin.z) / r.direction.z;
    if(x1 > x2) Swap(x1, x2);
    if(y1 > y2) Swap(y1, y2);
    if(z1 > z2) Swap(z1, z2);
    *tmin = Max(Max(x1, y1), z1);
    *tmax = Min(Min(x2, y2), z2);
    return *tmin <= *tmax;
}

Vec3 BBox::Size() const
{
    return this->max_point - this->min_point;
}

void BBox::Partition(Axis axis, double p, bool* left, bool* right) const
{
    switch(axis) {
        case Axis::X:
            *left   = this->min_point.x <= p;
            *right  = this->max_point.x >= p;
            break;
        case Axis::Y:
            *left   = this->min_point.y <= p;
            *right  = this->max_point.y >= p;
            break;
        case Axis::Z:
            *left   = this->min_point.z <= p;
            *right  = this->max_point.z >= p;
            break;
        case Axis::NONE: default:
            break;
    }
}

BBox SurroundingBBox(const std::vector<Surface*>& surfaces)
{
    if(surfaces.size() == 0) return {};
    BBox b = surfaces[0]->GetBBox();
    for(const auto& s : surfaces) {
        b = b.Union(s->GetBBox());
    }
    return b;
}
