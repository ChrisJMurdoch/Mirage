
#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <algorithm>

struct Pixel
{
    float r, g, b;

    constexpr Pixel(float r, float g, float b) : r(r), g(g), b(b)
    { }
    constexpr Pixel(glm::vec3 const v) : r(v.r), g(v.g), b(v.b)
    { }
    Pixel &operator+=(Pixel const &o)
    {
        r=r+o.r;
        g=g+o.g;
        b=b+o.b;
        return *this;
    }
    Pixel operator*(float f) const
    {
        return Pixel{ r*f, g*f, b*f };
    }
    Pixel operator/(float f) const
    {
        return Pixel{ r/f, g/f, b/f };
    }
    Pixel operator+(float f) const
    {
        return Pixel{ r+f, g+f, b+f };
    }
    Pixel operator-(float f) const
    {
        return Pixel{ r-f, g-f, b-f };
    }
    operator glm::vec3() const
    {
        return glm::vec3(r, g, b);
    }
};

class Image
{
private:
    int width, height, channels;
    std::vector<Pixel> pixels;

public:
    Image(int width, int height);
    Image(char const *filename);
    Image(Image const &other);
    Image(Image const &&other);
    Image &operator=(Image const &other);
    Image &operator=(Image const &&other);

    Pixel *operator[](std::size_t index);
    Pixel const *operator[](std::size_t index) const;

    int getWidth() const;
    int getHeight() const;
    void save(char const *filename) const;

    Image blur() const;

private:
    Pixel blurredPixel(int x, int y) const;
};

struct PhysicalMaterial
{
    Image const *normal;
    Image *lightmap;
};
