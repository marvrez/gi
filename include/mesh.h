#ifndef MESH_H
#define MESH_H

#include "triangle.h"
#include "bbox.h"
#include "kdtree.h"

#include <vector>
#include <memory>

class Material;
struct Vec3;
struct Mat4;

// triangle mesh
class Mesh : public Surface {
private:
    friend class Triangle;
    std::unique_ptr<BBox> bbox;
    std::unique_ptr<KDTree> tree;

    int num_triangles;
    std::vector<int> indices; // face indices
    std::vector<Vec3> positions, normals, texcoords;
    std::vector<Triangle> triangles; // triangle interfaces for shape intersection

    void Dirtify();
public:
    Mesh(const std::vector<Vec3>& positions, Material* material);
    Mesh(const std::vector<Vec3>& positions, const std::vector<Vec3>& normals, Material* material);
    Mesh(const std::vector<Vec3>& positions, const std::vector<Vec3>& normals, const std::vector<Vec3>& texcoords, Material* material);

    virtual BBox GetBBox();
    virtual bool Intersect(const Ray& r, Hit* h) const;
    virtual void Build();

    // mesh transformations
    void Transform(const Mat4& m);
    void Rotate(const Vec3& axis, double angle_rad);
    void MoveTo(const Vec3& pos, const Vec3& anchor=Vec3(0.5));
    void FitInside(const BBox& box, const Vec3& anchor=Vec3(0.5));
    void FitInsideUnitCube(); // inside [0, 1]
    void FitInsideBiUnitCube(); // inside [-1, +1]
    void MoveToCentre(); // move to centre of coordinate systme, i.e. position (0,0,0)

    void RepairNormals();
};

#endif
