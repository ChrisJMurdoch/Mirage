
#pragma once

#include <vector>

struct Pixel
{
    float r, g, b, a;
};

class Image
{
private:
    int width, height, channels;
    std::vector<Pixel> pixels;

public:
    Image(char const *filename);
    Pixel *operator[](std::size_t index);
};
