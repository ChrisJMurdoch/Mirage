
#pragma once

#include <vector>

struct Pixel
{
    float r, g, b;
    Pixel &operator+=(Pixel const &o)
    {
        r+=o.r;
        g+=o.g;
        b+=o.b;
        return *this;
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
