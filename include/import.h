#ifndef IMPORT_H
#define IMPORT_H

#include "mesh.h"

#include <string>
#include <memory>

class Material;

std::shared_ptr<Mesh> ReadSTL(std::string path, Material* material);
std::shared_ptr<Mesh> ReadOBJ(std::string path, Material* material);
std::shared_ptr<Mesh> ReadOFF(std::string path, Material* material);

#endif
