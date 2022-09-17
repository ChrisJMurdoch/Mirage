
#pragma once

#include <functional>

class Texture
{
private:
    unsigned int handle;

public:
    Texture(char const *filepath);
    ~Texture();
    Texture(Texture const &other) = delete;
    Texture &operator=(Texture const &other) = delete;

    void use(unsigned int channel, std::function<void()> operation) const;
};
