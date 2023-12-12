
#pragma once

#include <functional>
#include <optional>

class Texture
{
public:
    /// Values correspond with OpenGL GL_TEXTURE constants
    enum Channel
    {
        Albedo    = 0x84C0,
        Normal    = 0x84C1,
        Roughness = 0x84C2,
        Baked     = 0x84C3,
    };

private:
    unsigned int handle;

public:
    Texture(char const *filepath, bool verbose=false);
    Texture(Texture &&other);
    ~Texture();
    Texture(Texture const &other) = delete;
    Texture &operator=(Texture const &other) = delete;

    void use(Channel channel, std::function<void()> operation) const;
    void bind(Channel channel) const;
    void unbind(Channel channel) const;
};

struct Material
{
    std::optional<Texture> albedo, normal, roughness, baked;

    Material(std::optional<Texture> &&albedo, std::optional<Texture> &&normal, std::optional<Texture> &&roughness, std::optional<Texture> &&baked);
    Material(Material const &other) = delete;
    Material &operator=(Material const &other) = delete;
};
