
#include "display/objLoader.hpp"

#include "display/geometry.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <chrono>

/// Hashkey-compatible struct used for OBJ-style vertex data indices
struct Index
{
    unsigned int vert, tex, norm;
    bool operator==(Index const &other) const
    {
        return (vert==other.vert && tex==other.tex && norm==other.norm);
    }
};
namespace std
{
    template <>
    struct hash<Index>
    {
        std::size_t operator()(Index const &index) const
        {
            return ( (hash<unsigned int>()(index.vert) ^ (hash<unsigned int>()(index.tex) << 1)) >> 1) ^ (hash<unsigned int>()(index.norm) << 1 );
        }
    };
}

/// OBJ-format face
struct Face
{
    std::vector<Index> indices;
};

/// OpenGL-format triangle
struct Triangle
{
    Index a, b, c;
};

/// Split string/stream into vector using provided delimiter 
std::vector<std::string> split(std::istringstream &stream, char const delimiter)
{
    std::vector<std::string> out;
    std::string word;
    while(std::getline(stream, word, delimiter))
        out.push_back(word);
    return out;
}
std::vector<std::string> split(std::string const &in, char const delimiter)
{
    std::istringstream stream(in);
    return split(stream, delimiter);
}

/// Read OBJ-format data into vectors
void readData
(
    char const *filepath,
    std::vector<glm::vec3> &vertCoords,
    std::vector<glm::vec2> &texCoords,
    std::vector<glm::vec3> &normals,
    std::vector<Face> &faces
)
{
    // Open file
    std::ifstream file(filepath);
    std::string line;
    while (std::getline(file, line))
    {
        // Parse line using stream
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
        else if (command=="vn") // Parse normal
        {
            glm::vec3 norm;
            stream >> norm.x >> norm.y >> norm.z;
            normals.push_back(norm);
        }
        else if (command=="f") // Parse face
        {
            Face face{};

            // Process each index in face
            for (std::string code : split(stream, ' '))
            {
                // Split index into vert/tex/norm components
                std::vector<std::string> vals=split(code, '/');
                if (vals.size()<3)
                    continue;
                
                // Add index to face
                face.indices.push_back( Index
                    {
                        static_cast<unsigned int>(std::stoi(vals[0])-1), static_cast<unsigned int>(std::stoi(vals[1])-1), static_cast<unsigned int>(std::stoi(vals[2])-1)
                    }
                );
            }

            // Add face to vector
            faces.push_back(face);
        }
    }
}

/// Convert OBJ-format faces into OpenGL-compatible triangles using fan method
std::vector<Triangle> triangulate(std::vector<Face> const &faces)
{
    std::vector<Triangle> triangles;
    for (Face const &face : faces) // For each face
        for (int i=1; i<face.indices.size()-1; i++) // Use fan method to split face into individual triangles
            triangles.push_back( Triangle{ face.indices[i], face.indices[i+1], face.indices[0] } );
    
    return triangles;
}

std::pair<std::vector<Vertex>, std::vector<unsigned int>> objLoader::loadObj(char const *filepath, bool const verbose)
{
    auto loadStart = std::chrono::system_clock::now();

    // Load raw data from obj
    std::vector<glm::vec3> vertCoords;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<Face> faces;
    readData(filepath, vertCoords, texCoords, normals, faces);

    auto processStart = std::chrono::system_clock::now();

    // Triangulate faces
    std::vector<Triangle> triangles = triangulate(faces);

    // Map (vertIndex, texIndex) -> generated index
    std::unordered_map<Index, unsigned int> indexMap;

    // Create placeholder generated index mapping for each index permutation
    for (Triangle const &triangle : triangles)
    {
        indexMap[triangle.a] = 0;
        indexMap[triangle.b] = 0;
        indexMap[triangle.c] = 0;
    }

    // Generate vertex for each unique generated index
    std::vector<Vertex> vertices;
    for ( auto &entry : indexMap )
    {
        vertices.push_back( {vertCoords[entry.first.vert], texCoords[entry.first.tex], normals[entry.first.norm]} );
        entry.second = vertices.size() - 1;
    }

    // Generate index list using previous mapping
    std::vector<unsigned int> indices;
    for (Triangle const &triangle : triangles)
    {
        indices.push_back(indexMap[triangle.a]);
        indices.push_back(indexMap[triangle.b]);
        indices.push_back(indexMap[triangle.c]);
    }

    auto processEnd = std::chrono::system_clock::now();

    // Report load statistics
    if (verbose)
    {
        std::cout << "Loaded " << filepath << ":" << std::endl;
        std::cout << " - Duration:   ";
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(processStart-loadStart).count() << "ms(reading) + ";
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(processEnd-processStart).count() << "ms(processing)" << std::endl;
        std::cout << " - Processing: Converted " << faces.size() << " OBJ-format faces -> ";
        std::cout << triangles.size() << " OpenGL-format triangles" << std::endl;
        std::cout << " - Processing: Converted " << vertCoords.size() <<  "/" << texCoords.size()  <<  "/" << normals.size() << " OBJ-format vertices -> ";
        std::cout << vertices.size() << " OpenGL-format vertices" << std::endl << std::endl;
    }

    return std::pair{vertices, indices};
}
