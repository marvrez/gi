#include "import.h"

#include "vec3.h"
#include "mesh.h"
#include "material.h"

#include <stdio.h>

static inline int FileSize(FILE* fp)
{
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    return size;
}

static inline void IncrementFilePointer(FILE* fp, int offset) { fseek(fp, offset, SEEK_CUR); }

std::shared_ptr<Mesh> ReadSTL(std::string path, Material* material)
{
    FILE* fp = fopen(path.c_str(), "rb");
    int num_bytes = FileSize(fp);
    int num_triangles = (num_bytes - 84) / 50; // 84 header bytes, 50 bytes per triangle
    std::vector<Vec3> vertices(3*num_triangles);
    IncrementFilePointer(fp, 96); // skip 84 bytes header + 12 bytes of first triangle normal
    float buf[9];
    for(int i = 0; i < num_triangles; ++i) {
        fread(buf, sizeof(float), 9, fp); // triangle vertices
        int idx = 3*i;
        vertices[idx + 0] = { buf[0], buf[1], buf[2] };
        vertices[idx + 1] = { buf[3], buf[4], buf[5] };
        vertices[idx + 2] = { buf[6], buf[7], buf[8] };
        IncrementFilePointer(fp, 14); // skip 2 bytes of attribute byte count + 12 bytes of next triangle normal
    }
    fclose(fp);
    return std::make_shared<Mesh>(vertices, material);
}
