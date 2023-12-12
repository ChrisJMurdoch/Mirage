
#include "aether/image.hpp"

#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <exception>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <filesystem>

Image::Image(int width, int height) : width(width), height(height), channels(3)
{
    // Create float Pixels
    pixels.reserve(width*height);
    for (int i=0; i<width*height; i++)
        pixels.push_back( Pixel{0.0f, 0.0f, 0.0f} );
}

Image::Image(char const *filename)
{
    // Load data
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 3);
    if (data==nullptr)
        throw std::exception("stbi_load failed.");

    // Ensure correct #channels
    if (channels!=3)
        throw(std::exception("Data has wrong number of channels for PixelRGB"));

    // Convert data to float Pixels
    pixels.reserve(width*height);
    for (int i=0; i<width*height; i++)
    {
        unsigned char *ptr = data + i*channels;
        pixels.push_back( Pixel{ptr[0]/255.0f, ptr[1]/255.0f, ptr[2]/255.0f} );
    }

    // Free data
    stbi_image_free(data);
}

Image::Image(Image const &other) :width{other.width}, height{other.height}, channels{other.channels}, pixels{other.pixels}
{ }

Image::Image(Image const &&other) :width{other.width}, height{other.height}, channels{other.channels}, pixels{std::move(other.pixels)}
{ }

Image &Image::operator=(Image const &other)
{
    width = other.width;
    height = other.height;
    channels = other.channels;
    pixels = other.pixels;
    return *this;
}

Image &Image::operator=(Image const &&other)
{
    width = other.width;
    height = other.height;
    channels = other.channels;
    pixels = std::move(other.pixels);
    return *this;
}

Pixel *Image::operator[](std::size_t index)
{
    return &pixels[index*width];
}

Pixel const *Image::operator[](std::size_t index) const
{
    return &pixels[index*width];
}

int Image::getWidth() const
{
    return width;
}

int Image::getHeight() const
{
    return height;
}

unsigned char toChar(float x)
{
    return std::round( std::clamp(x, 0.0f, 1.0f) * 255 );
}

void Image::save(char const *filename) const
{
    // Generate data
    std::vector<unsigned char> data;
    data.reserve(width*height*channels);
    for (int i=0; i<width*height; i++)
    {
        data.push_back(toChar(pixels[i].r));
        data.push_back(toChar(pixels[i].g));
        data.push_back(toChar(pixels[i].b));
    }

    std::string path = std::string{filename};
    std::string loc = path.substr(0, path.find_last_of('/'));

    std::filesystem::create_directories(loc);
    stbi_flip_vertically_on_write(true);
    stbi_write_jpg(filename, width, height, channels, data.data(), 90);
}

Image Image::blur() const
{
    Image blurred{width, height};
    for (int y=0; y<height; y++)
        for (int x=0; x<width; x++)
            blurred[y][x] = blurredPixel(x, y);
    return blurred;
}

Pixel Image::blurredPixel(int x, int y) const
{
    int count = 0;
    Pixel sum{0, 0, 0};
    for (int yi=y-1; yi<y+2; yi++)
    {
        if (yi<0 || yi>=height)
            continue;
        for (int xi=x-1; xi<x+2; xi++)
        {
            if (xi<0 || xi>=width)
                continue;
            count++;
            sum += (*this)[yi][xi];
        }
    }
    return sum / count;
}
