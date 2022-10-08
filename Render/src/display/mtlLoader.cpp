
#include "display/mtlLoader.hpp"

#include "display/texture.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <optional>

std::optional<Texture> loadTexture(char const *filepath, bool verbose=false)
{
    // Don't load empty path
    if (std::strlen(filepath)==0)
        return std::nullopt;
    
    // try to load texture
    try
    {
        return std::make_optional<Texture>(filepath, verbose);
    }
    catch(const std::exception& e)
    {
        std::cout << "WARNING: Failed to load texture located at " << filepath << std::endl;
        return std::nullopt;
    }
}

Material mtlLoader::loadMtl(char const *filepath, bool const verbose)
{
    // Get .mtl directory path
    std::string path = std::string{filepath};
    std::string loc = path.substr(0, path.find_last_of('/')+1);
    std::string albedoPath="", normalPath="", roughnessPath="", bakedPath="";

    // Find texture locations
    std::ifstream file(filepath);
    std::string line;
    while (std::getline(file, line))
    {
        // Split line
        std::stringstream stream{line};
        std::string first, second;
        stream >> first >> second;

        // Set path
        if      ( first=="map_Kd" )    albedoPath    = loc+second;
        else if ( first=="map_Bump" )  normalPath    = loc+second;
        else if ( first=="map_Ns" )    roughnessPath = loc+second;
        else if ( first=="map_Baked" ) bakedPath     = loc+second;
    }

    // Load textures
    return Material
    {
        loadTexture(albedoPath.c_str(), verbose),
        loadTexture(normalPath.c_str(), verbose),
        loadTexture(roughnessPath.c_str(), verbose),
        loadTexture(bakedPath.c_str(), verbose)
    };
}
