
#include "engine/main.h"

#include "render/display.h"
#include "utility/io.h"
#include "generate/mesh.h"
#include "generate/noise.h"
#include "model/virtualVector.h"
#include "model/instance.h"

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
        const int terrainVertices = 600;
        VertexArray terrainVertexArray(mesh::vertexCount(terrainVertices));
        IndexArray terrainIndexArray(mesh::triangleCount(terrainVertices));
        mesh::generateCube(terrainVertices, terrainVertexArray, terrainIndexArray);
        mesh::planet(terrainVertexArray, 10, 16);
        mesh::fixNormals(terrainVertexArray, terrainIndexArray);
        Model terrainModel = Model(&terrainVertexArray, &terrainIndexArray, &terrain);

        // Add instances
        Instance planet = Instance(terrainModel);
        display.addInstance(&planet);

        // Run display
        while (!display.shouldClose())
        {
            planet.physics();
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
