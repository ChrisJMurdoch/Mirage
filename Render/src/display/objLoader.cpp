
#include "display/objLoader.hpp"

#include "display/geometry.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <chrono>

struct Index
{
    unsigned int vert, tex;
};

struct Face
{
    Index a, b, c;
};

std::vector<std::string> split(std::string const &in, char const delimiter)
{
    std::vector<std::string> out;
    std::stringstream stream(in);
    std::string word;
    while(std::getline(stream, word, delimiter))
        out.push_back(word);
    return out;
}

void readData
(
    char const *filepath,
    std::vector<glm::vec3> &vertCoords,
    std::vector<glm::vec2> &texCoords,
    std::vector<Face> &faces
)
{
    // Open file
    std::ifstream file(filepath);
    std::string line;
    while (std::getline(file, line))
    {
        // Parse line using string stream
        std::istringstream stream(line);

        // Switch on first character of line
        std::string command;
        stream >> command;
        if (command=="v") // Parse vertex coordinate
        {
            glm::vec3 vec;
            stream >> vec.x >> vec.y >> vec.z;
            vertCoords.push_back(vec);
        }
        else if (command=="vt") // Parse texture coordinate
        {
            glm::vec2 tex;
            stream >> tex.x >> tex.y;
            texCoords.push_back(tex);
        }
        else if (command=="f") // Parse face
        {
            // Read face vertex strings
            std::string f1, f2, f3;
            stream >> f1 >> f2 >> f3;
            std::vector<std::string> s1=split(f1, '/'), s2=split(f2, '/'), s3=split(f3, '/');

            // Construct face
            faces.push_back
            (
                {
                    { static_cast<unsigned int>(std::stoi(s1[0])-1), static_cast<unsigned int>(std::stoi(s1[1])-1) },
                    { static_cast<unsigned int>(std::stoi(s2[0])-1), static_cast<unsigned int>(std::stoi(s2[1])-1) },
                    { static_cast<unsigned int>(std::stoi(s3[0])-1), static_cast<unsigned int>(std::stoi(s3[1])-1) }
                }
            );
        }
    }
}

std::pair<std::vector<Vertex>, std::vector<unsigned int>> objLoader::loadObj(char const *filepath, bool const verbose)
{
    auto loadStart = std::chrono::system_clock::now();

    // Load raw data from obj
    std::vector<glm::vec3> vertCoords;
    std::vector<glm::vec2> texCoords;
    std::vector<Face> faces;
    readData(filepath, vertCoords, texCoords, faces);

    auto processStart = std::chrono::system_clock::now();

    // Map (vertIndex, texIndex) -> generated index
    std::unordered_map<unsigned int, std::unordered_map<unsigned int, unsigned int>> indexMap;

    // Create placeholder generated index mapping for each index permutation
    for (Face const &face : faces)
    {
        indexMap[face.a.vert][face.a.tex] = 0;
        indexMap[face.b.vert][face.b.tex] = 0;
        indexMap[face.c.vert][face.c.tex] = 0;
    }

    // Generate vertex for each unique generated index
    std::vector<Vertex> vertices;
    for ( auto const &vertEntry : indexMap )
    {
        for ( auto &texEntry : indexMap[vertEntry.first] )
        {
            vertices.push_back( {vertCoords[vertEntry.first], texCoords[texEntry.first]} );
            texEntry.second = vertices.size() - 1;
        }
    }

    // Generate index list using previous mapping
    std::vector<unsigned int> indices;
    for (Face const &face : faces)
    {
        indices.push_back(indexMap[face.a.vert][face.a.tex]);
        indices.push_back(indexMap[face.b.vert][face.b.tex]);
        indices.push_back(indexMap[face.c.vert][face.c.tex]);
    }

    auto processEnd = std::chrono::system_clock::now();

    // Report load statistics
    if (verbose)
    {
        std::cout << "Loaded " << filepath << ":" << std::endl;
        std::cout << " - Duration:   ";
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(processStart-loadStart).count() << "ms(reading) + ";
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(processEnd-processStart).count() << "ms(processing)" << std::endl;
        std::cout << " - Processing: Converted " << vertCoords.size() <<  "/" << texCoords.size() << " OBJ-format vertices -> ";
        std::cout << vertices.size() << " OpenGL-format vertices" << std::endl << std::endl;
    }

    return std::pair{vertices, indices};
}
