
#include "engine/main.hpp"

#include "render/display.hpp"
#include "utility/io.hpp"
#include "generate/mesh.hpp"
#include "generate/noise.hpp"
#include "model/virtualVector.hpp"
#include "model/instance.hpp"
#include "engine/planet.hpp"

#include <chrono>
#include <iostream>
#include <exception>
#include <unordered_map>

int main()
{
    try
    {
        // Create display
        Display display = Display(1000, 600, "Redshift");

        // Create programs
        Program post = Program(), terrain = Program();
        addShaders(post, "shaders\\post.vert", "shaders\\post.frag");
        addShaders(terrain, "shaders\\terrain.vert", "shaders\\terrain.frag");

        // Add post-processing to display
        display.addPostProgram(&post);

        // Create planet
        Planet planet(10);
        const int terrainVertices = 600;
        VertexArray terrainVertexArray(mesh::vertexCount(terrainVertices));
        IndexArray terrainIndexArray(mesh::triangleCount(terrainVertices));
        mesh::generateCube(terrainVertices, terrainVertexArray, terrainIndexArray);
        mesh::setPositions(terrainVertexArray, &planet, 16);
        mesh::fixNormals(terrainVertexArray, terrainIndexArray);
        mesh::setColours(terrainVertexArray, &planet, 16);
        Model terrainModel(&terrainVertexArray, &terrainIndexArray, &terrain);

        // Add instances
        Instance planetInstance = Instance(terrainModel);
        display.addInstance(&planetInstance);

        // Run display
        while (!display.shouldClose())
        {
            planetInstance.physics();
            display.render();
        }

        return 0;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
}

void addShaders(Program &prog, char const *vert, char const *frag)
{
    // Add vertex shader
    std::string *vertexSource = io::read(vert);
    prog.addShader(vertexSource->c_str(), Program::Shader::VERTEX);
    delete vertexSource;

    // Add fragment shader
    std::string *fragmentSource = io::read(frag);
    prog.addShader(fragmentSource->c_str(), Program::Shader::FRAGMENT);
    delete fragmentSource;

    // Link shaders into program
    prog.link();
}
