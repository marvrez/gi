#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec3.h"

#include "utils.h"

#include <memory>
#include <string>

class Texture {
public:
    virtual Vec3 Sample(double u, double v, const Vec3& p) const = 0;
    virtual ~Texture() {};
};

class SolidTexture : public Texture {
private:
    Vec3 color;
public:
    SolidTexture() : color(0) {}
    SolidTexture(const Vec3& color) : color(color) {}
    virtual Vec3 Sample(double u, double v, const Vec3& p) const { return color; }
};

class CheckeredTexture : public Texture {
private:
    std::shared_ptr<Texture> a, b;
    double freq;
public:
    CheckeredTexture(double size=0.5) : CheckeredTexture(new SolidTexture(0), new SolidTexture(1), size) {}
    CheckeredTexture(Texture* a, Texture* b, double size=0.5) : a(a), b(b), freq(2*M_PI / size) {}
    virtual Vec3 Sample(double u, double v, const Vec3& p) const;
};

class GridTexture : public Texture {
private:
    std::shared_ptr<Texture> a, b;
    double spacing, width;
public:
    GridTexture(Texture* a, Texture* b, double spacing, double width) : a(a), b(b), spacing(spacing), width(width) {}
    virtual Vec3 Sample(double u, double v, const Vec3& p) const;
};

class ImageTexture : public Texture {
private:
    unsigned char* data;
    int height, width, channels;
public:
    ImageTexture() = delete;
    ImageTexture(std::string filename);
    ImageTexture(unsigned char* data, int height, int width) : data(data), height(height), width(width) {}
    ~ImageTexture() { delete[] this->data; }
    bool IsValid() const { return this->data != nullptr; };
    virtual Vec3 Sample(double u, double v, const Vec3& p) const;
};

#endif
