
#include "render/texture.hpp"

#include "render/gl.hpp"

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
        // Texture wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Texture scaling
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Anisotropic filtering
        float maxAnisotropy = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

        // Copy over data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        // Generate mipmap
        glGenerateMipmap(GL_TEXTURE_2D);
    });
    
    auto bufferEnd = std::chrono::system_clock::now();

    if (verbose)
    {
        long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(bufferEnd-loadStart).count();
        std::cout << "Loaded texture " << filepath << " (" << ms << "ms)" << std::endl;
    }

    // Free image data
    stbi_image_free(data);
}

Texture::Texture(Texture &&other) : handle(other.handle)
{
    other.handle = 0;
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

Material::Material(std::optional<Texture> &&albedo, std::optional<Texture> &&normal, std::optional<Texture> &&roughness, std::optional<Texture> &&baked)
    : albedo{std::move(albedo)}, normal{std::move(normal)}, roughness{std::move(roughness)}, baked{std::move(baked)}
{ }
