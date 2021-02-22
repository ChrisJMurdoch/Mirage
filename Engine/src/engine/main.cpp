
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

        // Create terrain program
        Program terrain = Program();
        {
            // Add vertex shader
            std::string *vertexSource = io::read("shaders\\terrain.vert");
            terrain.addShader(vertexSource->c_str(), Program::Shader::VERTEX);
            delete vertexSource;

            // Add fragment shader
            std::string *fragmentSource = io::read("shaders\\terrain.frag");
            terrain.addShader(fragmentSource->c_str(), Program::Shader::FRAGMENT);
            delete fragmentSource;

            // Link shaders into program
            terrain.link();
        }

        // Create terrain program
        Program water = Program();
        {
            // Add vertex shader
            std::string *vertexSource = io::read("shaders\\water.vert");
            water.addShader(vertexSource->c_str(), Program::Shader::VERTEX);
            delete vertexSource;

            // Add fragment shader
            std::string *fragmentSource = io::read("shaders\\water.frag");
            water.addShader(fragmentSource->c_str(), Program::Shader::FRAGMENT);
            delete fragmentSource;

            // Link shaders into program
            water.link();
        }

        // Create planet
        const int edgeVerticesA = 500;
        VertexArray vertexArrayA(mesh::vertexCount(edgeVerticesA));
        IndexArray indexArrayA(mesh::triangleCount(edgeVerticesA));
        mesh::generateCube(edgeVerticesA, vertexArrayA, indexArrayA);
        mesh::planet(vertexArrayA, 10, 16);
        mesh::fixNormals(vertexArrayA, indexArrayA);
        Model modelA = Model(&vertexArrayA, &indexArrayA, &terrain);

        // Create sea
        const int edgeVerticesB = 50;
        VertexArray vertexArrayB(mesh::vertexCount(edgeVerticesB));
        IndexArray indexArrayB(mesh::triangleCount(edgeVerticesB));
        mesh::generateCube(edgeVerticesB, vertexArrayB, indexArrayB);
        mesh::sea(vertexArrayB, 16);
        mesh::fixNormals(vertexArrayB, indexArrayB);
        Model modelB = Model(&vertexArrayB, &indexArrayB, &water);

        // Add instances
        Instance planet(modelA);
        Instance sea(modelB);
        display.addInstance(&planet);
        display.addInstance(&sea);

        // Run display
        while (!display.shouldClose())
        {
            planet.physics();
            sea.physics();
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
