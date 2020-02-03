#include "export.h"

#include "vec3.h"

#include <stdlib.h>
#include <stdio.h>

void SaveSTL(std::string path, const std::vector<int>& indices, const std::vector<Vec3>& vertices)
{
    unsigned num_triangles = indices.size() / 3;
    unsigned num_bytes = 50*num_triangles + 84;
    std::vector<char> out_buffer(num_bytes, 0);
    memcpy(&out_buffer[80], &num_triangles, sizeof(unsigned));
    for(unsigned i = 0; i < num_triangles; ++i) {
        int i0 = indices[3*i], i1 = indices[3*i + 1], i2 = indices[3*i + 2];
        Vec3 v0 = vertices[i0], v1 = vertices[i1], v2 = vertices[i2];
        Vec3 normal = Normalized(Cross(v1-v0, v2-v0));

        unsigned idx = 50*i + 84;
        constexpr unsigned vec_bytes = 3*sizeof(float);
        float n_arr[]  = { float(normal.x), float(normal.y), float(normal.z) };
        float v0_arr[] = { float(v0.x), float(v0.y), float(v0.z) };
        float v1_arr[] = { float(v1.x), float(v1.y), float(v1.z) };
        float v2_arr[] = { float(v2.x), float(v2.y), float(v2.z) };
        memcpy(&out_buffer[idx],               n_arr,  vec_bytes);
        memcpy(&out_buffer[idx + vec_bytes],   v0_arr, vec_bytes);
        memcpy(&out_buffer[idx + 2*vec_bytes], v1_arr, vec_bytes);
        memcpy(&out_buffer[idx + 3*vec_bytes], v2_arr, vec_bytes);
    }
    FILE* fp = fopen(path.c_str(), "wb");
    fwrite(out_buffer.data(), sizeof(char), num_bytes, fp);
    fclose(fp);
}
