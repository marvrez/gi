#ifndef EXPORT_H
#define EXPORT_H

#include <vector>

struct Vec3;

void SaveSTL(const char* path, const std::vector<int>& indices, const std::vector<Vec3>& vertices);

#endif
