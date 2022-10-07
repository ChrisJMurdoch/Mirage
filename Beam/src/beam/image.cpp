
#include "beam/image.hpp"

#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <exception>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <filesystem>

Image::Image(char const *filename)
{
    // Load data
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

Pixel *Image::operator[](std::size_t index)
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
    unsigned char scaled = std::round( x * 255 );
    return std::clamp( scaled, static_cast<unsigned char>(0), static_cast<unsigned char>(255) );
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
    stbi_write_jpg(filename, width, height, channels, data.data(), 90);
}
