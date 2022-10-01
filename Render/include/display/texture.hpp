
#pragma once

#include <functional>

class Texture
{
public:
    /// Values correspond with OpenGL GL_TEXTURE constants
    enum Channel
    {
        Albedo    = 0x84C0,
        Normal    = 0x84C1,
        Roughness = 0x84C2,
    };

private:
    unsigned int handle;

public:
    Texture(char const *filepath);
    ~Texture();
    Texture(Texture const &other) = delete;
    Texture &operator=(Texture const &other) = delete;

    void use(Channel channel, std::function<void()> operation) const;
    void bind(Channel channel) const;
    void unbind(Channel channel) const;
};
