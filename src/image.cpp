#include "image.h"

#include "utils.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>

constexpr double GAMMA_INV = 1.0 / 2.2;

void Image::AddPixel(int x, int y, const Rgb& val)
{
    Pixel* p = &this->data[y*width + x];
    p->num_samples++;
    p->val = p->val + (val - p->val) / p->num_samples; // x_hat' = (n*x_hat + x_new) / (n+1)
}

static inline std::vector<unsigned char> GetRgbBytes(const std::vector<Pixel>& pixels)
{
    int n = pixels.size();
    std::vector<unsigned char> bytes(3*n);
    for(int i = 0; i < n; ++i) {
        Rgb rgb = Pow(pixels[i].val, GAMMA_INV); // gamma correction
        int idx = 3*i;
        bytes[idx]      = (unsigned char)(Clamp(255*rgb[0], 0., 255.));
        bytes[idx + 1]  = (unsigned char)(Clamp(255*rgb[1], 0., 255.));
        bytes[idx + 2]  = (unsigned char)(Clamp(255*rgb[2], 0., 255.));
    }
    return bytes;
}

bool Image::SaveJPG(const std::string& filename, int quality) const
{
    std::vector<unsigned char> bytes = GetRgbBytes(this->data);
    int success = stbi_write_jpg(filename.c_str(), width, height, 3, bytes.data(), quality);
    return success;
}

bool Image::SavePNG(const std::string& filename) const
{
    std::vector<unsigned char> bytes = GetRgbBytes(this->data);
    int success = stbi_write_png(filename.c_str(), width, height, 3, bytes.data(), 3*width);
    return success;
}

bool Image::SavePPM(const std::string& filename) const
{
    std::vector<unsigned char> bytes = GetRgbBytes(this->data);
    FILE* fp = fopen(filename.c_str(), "wb");
    fprintf(fp, "P6\n%d %d\n%d\n", width, height, 255);
    fwrite(bytes.data(), sizeof(unsigned char), bytes.size(), fp);
    fclose(fp);
    return true;
}
