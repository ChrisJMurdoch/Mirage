
#pragma once

#include "display/texture.hpp"

namespace mtlLoader
{
    /// Loads and processes .mtl file and returns a Material with the relevant textures
    Material loadMtl(char const *filepath, bool const verbose=false);
}
