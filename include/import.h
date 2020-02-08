#ifndef IMPORT_H
#define IMPORT_H

#include "mesh.h"

#include <memory>

class Material;

std::shared_ptr<Mesh> ReadSTL(const char* path, Material* material);
std::shared_ptr<Mesh> ReadOBJ(const char* path, Material* material);
std::shared_ptr<Mesh> ReadOFF(const char* path, Material* material);

#endif
