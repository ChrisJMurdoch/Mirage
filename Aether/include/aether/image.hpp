
#pragma once

#include <vector>

struct Pixel
{
    float r, g, b;
};

class Image
{
private:
    int width, height, channels;
    std::vector<Pixel> pixels;

public:
    Image(char const *filename);
    Pixel *operator[](std::size_t index);

    int getWidth() const;
    int getHeight() const;
    void save(char const *filename) const;
};
