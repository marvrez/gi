#include "texture.h"

#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Vec3 CheckeredTexture::Sample(double u, double v, const Vec3& p) const
{
    double f = this->freq;
    double s = sin(f*p.x)*sin(f*p.y)*sin(f*p.z);
    return s < 0 ? a->Sample(u, v, p) : b->Sample(u, v, p);
}

Vec3 GridTexture::Sample(double u, double v, const Vec3& p) const
{
    Vec3 q = Abs(Fract(p / spacing - 0.5) - 0.5);
    double line = Min(q.x, q.z);
    return line > width / 2 ? a->Sample(u, v, p) : b->Sample(u, v, p);
}

ImageTexture::ImageTexture(const char* filename)
{
    printf("Loading texture: %s\n", filename);
    this->data = stbi_load(filename, &width, &height, &channels, 0);
    if(!this->data) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        exit(0);
    }
}

Vec3 ImageTexture::Sample(double u, double v, const Vec3& p) const
{
     int x = Clamp(u*width, 0.0, width-1.0), y = Clamp((1-v)*height-0.001, 0.0, height-1.0);
     int idx = 3*(y*width + x);
     double r = int(data[idx]  ) / 255.0;
     double g = int(data[idx+1]) / 255.0;
     double b = int(data[idx+2]) / 255.0;
     return Vec3(r, g, b);
}
