
#include "display/texture.hpp"

#include "display/gl.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <exception>
#include <iostream>
#include <chrono>

Texture::Texture(char const *filepath, bool verbose)
{
    // Generate texture
    glGenTextures(1, &handle);

    auto loadStart = std::chrono::system_clock::now();

    // Load from file
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filepath, &width, &height, &channels, 0);
    if (data==nullptr)
        throw std::exception("stbi_load failed.");

    auto bufferStart = std::chrono::system_clock::now();

    // Bind (doesn't matter which channel)
    use(Channel::Albedo, [&]()
    {
        // Texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Copy over data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        // Generate mipmap
        glGenerateMipmap(GL_TEXTURE_2D);
    });
    
    auto bufferEnd = std::chrono::system_clock::now();

    if (verbose)
    {
        std::cout << "Loaded texture " << filepath << ":" << std::endl;
        std::cout << " - Duration:   ";
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(bufferStart-loadStart).count() << "ms(reading) + ";
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(bufferEnd-bufferStart).count() << "ms(GPU memcpy)" << std::endl << std::endl;
    }

    // Free image data
    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &handle);
}

void Texture::use(Channel channel, std::function<void()> operation) const
{
    gl::activeTexture(channel);
    gl::bindTexture2D(handle);
    operation();
    gl::bindTexture2D(0);
    gl::activeTexture(0);
}

void Texture::bind(Channel channel) const
{
    gl::activeTexture(channel);
    gl::bindTexture2D(handle);
    gl::activeTexture(0);
}

void Texture::unbind(Channel channel) const
{
    gl::activeTexture(channel);
    gl::bindTexture2D(0);
    gl::activeTexture(0);
}
