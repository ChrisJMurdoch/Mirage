
#include "display/texture.hpp"

#include "display/gl.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <exception>

Texture::Texture(char const *filepath)
{
    // Generate texture
    glGenTextures(1, &handle);

    // Load from file
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filepath, &width, &height, &channels, 0);
    if (data==nullptr)
        throw std::exception("stbi_load failed.");

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
