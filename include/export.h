#ifndef EXPORT_H
#define EXPORT_H

#include <string>
#include <vector>

struct Vec3;

void SaveSTL(std::string path, const std::vector<int>& indices, const std::vector<Vec3>& vertices);

#endif
