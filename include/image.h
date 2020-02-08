#ifndef IMAGE_H
#define IMAGE_H

#include "vec3.h"

#include <vector>

struct Pixel {
    int num_samples;
    Rgb val;
};

class Image {
private:
    int width, height;
    std::vector<Pixel> data;

public:
    Image() : width(0), height(0), data(0) { }
    Image(int width, int height) : width(width), height(height), data(width*height) { }

    void AddPixel(int x, int y, const Rgb& val);
    inline Rgb GetPixel(int x, int y) const { return data[y*width + x].val; }

    int Width()     const   { return width; }
    int Height()    const   { return height; }

    bool SaveJPG(const char* filename, int quality=90) const;
    bool SavePNG(const char* filename)  const;
    bool SavePPM(const char* filename)  const;
};

#endif
