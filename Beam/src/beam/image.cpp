
#include "beam/image.hpp"

#include <stb_image.h>

#include <exception>

Image::Image(char const *filename)
{
    // Load data
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    if (data==nullptr)
        throw std::exception("stbi_load failed.");

    // Ensure correct #channels
    if (channels!=4)
        throw("Data has wrong number of channels for PixelRGBA");

    // Convert data
    for (int i=0; i<width*height; i++)
    {
        unsigned char *ptr = data + i*channels;
        pixels.push_back( Pixel{ptr[0]/255.0f, ptr[1]/255.0f, ptr[2]/255.0f, ptr[3]/255.0f} );
    }

    // Free data
    stbi_image_free(data);
}

Pixel *Image::operator[](std::size_t index)
{
    return &pixels[index*width];
}
