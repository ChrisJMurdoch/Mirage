
#include "display/objLoader.hpp"

#include "display/geometry.hpp"

#include <fstream>
#include <sstream>
#include <string>
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

glm::vec3 readVec3(char const *line)
{
    glm::vec3 vec{};
    char const *next = line;

    vec.x = std::stof(next);
    next = strchr(next, ' ') + 1;
    vec.y = std::stof(next);
    next = strchr(next, ' ') + 1;
    vec.z = std::stof(next);

    return vec;
}

glm::vec2 readVec2(char const *line)
{
    glm::vec2 vec{};
    char const *next = line;

    vec.x = std::stof(next);
    next = strchr(next, ' ') + 1;
    vec.y = std::stof(next);

    return vec;
}

Index readIndex(char const *start)
{
    Index index{};
    char const *next = start;

    index.vert = std::stoi(next) - 1;
    next = strchr(next, '/') + 1;
    index.tex = std::stoi(next) - 1;
    next = strchr(next, '/') + 1;
    index.norm = std::stoi(next) - 1;

    return index;
}

Face readFace(char const *line)
{
    std::vector<Index> indices;
    char const *word = line;

    while(word!=nullptr)
    {
        indices.push_back( readIndex(word) );

        word = strchr(word, ' ');
        if (word!=nullptr) word++;
    }

    return Face{indices};
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
        // Get character pointer
        char const *lPtr = line.c_str();

        // Parse vertex coordinate
        if ( std::strncmp(lPtr, "v ", 2)==0 ) 
            vertCoords.push_back( readVec3(&lPtr[2]) );
        
        // Parse texture coordinate
        else if ( std::strncmp(lPtr, "vt ", 3)==0 ) 
            texCoords.push_back( readVec2(&lPtr[3]) );
        
        // Parse normal
        else if ( std::strncmp(lPtr, "vn ", 3)==0 ) 
            normals.push_back( readVec3(&lPtr[3]) );
        
        // Parse face
        else if ( std::strncmp(lPtr, "f ", 2)==0 ) 
            faces.push_back( readFace(&lPtr[2]) );
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

    // Generate vertex for each unique generated index (leave tangents and bitangents for later)
    std::vector<Vertex> vertices;
    for ( auto &entry : indexMap )
    {
        vertices.push_back( {vertCoords[entry.first.vert], texCoords[entry.first.tex], normals[entry.first.norm], glm::vec3{}, glm::vec3{}} );
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

    // Calculate tangents and bitangents
    {
        std::unordered_map<unsigned int, unsigned int> vertexOccurrences;

        // Accumulate tangents and bitangents
        for (int i=0; i<indices.size(); i+=3)
        {
            // Get triangle corners
            unsigned int i1=indices[i+0], i2=indices[i+1], i3=indices[i+2];
            Vertex &v1=vertices[i1], &v2=vertices[i2], &v3=vertices[i3];

            // Increment occurrences
            vertexOccurrences[i1]++;
            vertexOccurrences[i2]++;
            vertexOccurrences[i3]++;

            // Calculate tangent and bitangent
            glm::vec3 posDelta1 = v2.position - v1.position;
            glm::vec3 posDelta2 = v3.position - v1.position;
            glm::vec2 texDelta1 = v2.texturePosition - v1.texturePosition;
            glm::vec2 texDelta2 = v3.texturePosition - v1.texturePosition;
            float r = 1.0 / (texDelta1.x * texDelta2.y - texDelta1.y * texDelta2.x);
            glm::vec3 tangent = (posDelta1 * texDelta2.y - posDelta2 * texDelta1.y) * r;
            glm::vec3 bitangent = (posDelta2 * texDelta1.x - posDelta1 * texDelta2.x) * r;

            // Increment vertex accumulations
            v1.tangent += tangent; v2.tangent += tangent; v3.tangent += tangent;
            v1.bitangent += bitangent; v2.bitangent += bitangent; v3.bitangent += bitangent;
        }

        // Divide accumulations into averaged values
        for (int i=0; i<vertices.size(); i++)
        {
            int occurrences = vertexOccurrences[i];
            vertices[i].tangent /= occurrences;
            vertices[i].bitangent /= occurrences;
        }
    }

    auto processEnd = std::chrono::system_clock::now();

    // Report load statistics
    if (verbose)
    {
        long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(processEnd-loadStart).count();
        std::cout << "Loaded model " << filepath << " (" << ms << "ms)" << std::endl;
    }

    return std::pair{vertices, indices};
}
