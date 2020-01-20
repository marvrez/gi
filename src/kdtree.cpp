#include "kdtree.h"

#include "utils.h"
#include "surface.h"
#include "hit.h"
#include "ray.h"

#include <stdio.h>
#include <algorithm>

static inline double Median(std::vector<double>& v)
{
    auto begin = v.begin(), end = v.end();
    auto middle = begin + (end - begin) / 2; // middle for odd-length, upper-middle for even length
    std::nth_element(begin, middle, end);
    if((end - begin) % 2 != 0) return *middle;
    else {
        auto lower_middle = std::max_element(begin, middle); // max of lower half
        return 0.5*(*middle + *lower_middle);
    }
}

void KDNode::Split(int depth)
{
    int n = this->surfaces.size();
    if(n < 8) return; // too much overhead of tree traversal if there are few primitives in the scene
    std::vector<double> x(2*n), y(2*n), z(2*n);
    for(int i = 0; i < n; ++i) {
        BBox b = this->surfaces[i]->GetBBox();
        int idx = 2*i;
        x[idx] = b.min_point.x, x[idx+1] = b.max_point.x;
        y[idx] = b.min_point.y, y[idx+1] = b.max_point.y;
        z[idx] = b.min_point.z, z[idx+1] = b.max_point.z;
    }
    double mx = Median(x), my = Median(y), mz = Median(z); // calculate medians for x,y and z positions

    // best axis based on partition side with fewest points
    int best_size = BALANCING_FACTOR*n;
    Axis best_axis = Axis::NONE;
    double best_pos = 0.0;
    int count_x = this->PartitionCount(Axis::X, mx), count_y = this->PartitionCount(Axis::Y, my), count_z = this->PartitionCount(Axis::Z, mz);
    if(count_x < best_size) { best_size = count_x, best_axis = Axis::X, best_pos = mx; }
    if(count_y < best_size) { best_size = count_y, best_axis = Axis::Y, best_pos = my; }
    if(count_z < best_size) { best_size = count_z, best_axis = Axis::Z, best_pos = mz; }
    if(best_axis == Axis::NONE) return; // leaf node
    std::vector<Surface*> left_surfaces, right_surfaces;
    this->Partition(best_axis, best_pos, best_size, &left_surfaces, &right_surfaces);
    this->axis  = best_axis, this->pos = best_pos;
    this->left  = std::make_unique<KDNode>(left_surfaces);
    this->right = std::make_unique<KDNode>(right_surfaces);
    this->left->Split(depth+1);
    this->right->Split(depth+1);
    this->surfaces.clear(); // only leaf nodes contains surfaces
}

bool KDNode::Intersect(const Ray& r, Hit* h)
{
    Hit tmp;
    bool did_hit = false;
    for(Surface* s : this->surfaces) {
        if(s->Intersect(r, &tmp)){
            did_hit = true;
            *h = tmp;
        }
    }
    return did_hit && h->t > M_EPS && h->t < M_INF;
}

bool KDNode::Intersect(const Ray& r, double tmin, double tmax, Hit* h)
{
    double tsplit;
    bool left_first;
    switch(this->axis) {
        case Axis::X:
            tsplit = (this->pos - r.origin.x) / r.direction.x;
            left_first = (r.origin.x < this->pos) || (r.origin.x == this->pos && r.direction.x <= 0);
            break;
        case Axis::Y:
            tsplit = (this->pos - r.origin.y) / r.direction.y;
            left_first = (r.origin.y < this->pos) || (r.origin.y == this->pos && r.direction.y <= 0);
            break;
        case Axis::Z:
            tsplit = (this->pos - r.origin.z) / r.direction.z;
            left_first = (r.origin.z < this->pos) || (r.origin.z == this->pos && r.direction.z <= 0);
            break;
        case Axis::NONE: default:
            return this->Intersect(r, h);
    }
    KDNode* first   = left_first ? this->left.get()   : this->right.get();
    KDNode* second  = left_first ? this->right.get()  : this->left.get();
    if(tsplit > tmax || tsplit <= 0) return first->Intersect(r, tmin, tmax, h);
    else if(tsplit < tmin) return second->Intersect(r, tmin, tmax, h);
    else {
        Hit h1, h2;
        bool did_hit = first->Intersect(r, tmin, tsplit, &h1);
        if(h1.t < tsplit) *h = h1;
        else {
            if(second->Intersect(r, tsplit, Min(tmax, h1.t), &h2)) did_hit = true;
            *h = h1.t <= h2.t ? h1 : h2;
        }
        return did_hit;
    }
}

int KDNode::Partition(Axis axis, double p, int size, std::vector<Surface*>* left, std::vector<Surface*>* right)
{
    int left_count = 0, right_count = 0;
    bool is_left, is_right;
    if(left) left->reserve(size);
    if(right) right->reserve(size);
    for(Surface* s : surfaces) {
        s->GetBBox().Partition(axis, p, &is_left, &is_right);
        if(is_left) {
            if(left) left->push_back(s);
            ++left_count;
        }
        if(is_right) {
            if(right) right->push_back(s);
            ++right_count;
        }
    }
    return Max(left_count, right_count);
}

KDTree::KDTree(const std::vector<Surface*>& surfaces)
{
    printf("building k-d tree from %ld surfaces...", surfaces.size());
    double t1 = TimeNow();
    this->bbox = SurroundingBBox(surfaces);
    this->root = std::make_unique<KDNode>(surfaces);
    this->root->Split(0);
    double t2 = TimeNow();
    printf(" took %f seconds\n", t2-t1);
}

bool KDTree::Intersect(const Ray& r, Hit* h)
{
    double tmin, tmax;
    bool hit = this->bbox.Intersect(r, &tmin, &tmax);
    if(!hit || tmin > tmax || tmax <= 0) return false;
    return this->root->Intersect(r, tmin, tmax, h);
}
