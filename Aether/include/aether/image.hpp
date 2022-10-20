
#pragma once

#include <atomic>
#include <vector>
#include <algorithm>

struct Pixel
{
    float r, g, b;
    Pixel(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    Pixel &operator+=(Pixel const &o)
    {
        r=std::clamp(r+o.r, 0.0f, 1.0f);
        g=std::clamp(g+o.g, 0.0f, 1.0f);
        b=std::clamp(b+o.b, 0.0f, 1.0f);
        return *this;
    }
    Pixel operator*(float f)
    {
        return Pixel
        {
            std::clamp(r*f, 0.0f, 1.0f),
            std::clamp(g*f, 0.0f, 1.0f),
            std::clamp(b*f, 0.0f, 1.0f)
        };
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
    Image(Image const &other) = delete;
    Image &operator=(Image const &other) = delete;

    Pixel *operator[](std::size_t index);

    int getWidth() const;
    int getHeight() const;
    void save(char const *filename) const;
};
