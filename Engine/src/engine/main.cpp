
#include "engine/main.h"

#include "render/display.h"
#include "utility/io.h"
#include "generate/mesh.h"
#include "generate/noise.h"
#include "model/virtualVector.h"

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
        
        // Create program
        Program prog = Program();
        {
            // Add vertex shader
            std::string *vertexSource = io::read("shaders\\shader.vert");
            prog.addShader(vertexSource->c_str(), Program::Shader::VERTEX);
            delete vertexSource;

            // Add fragment shader
            std::string *fragmentSource = io::read("shaders\\shader.frag");
            prog.addShader(fragmentSource->c_str(), Program::Shader::FRAGMENT);
            delete fragmentSource;

            // Link shaders into program
            prog.link();
        }

        // Create arrays
        const int edgeVertices = 500;
        VertexArray vertexArray(mesh::vertexCount(edgeVertices));
        IndexArray indexArray(mesh::triangleCount(edgeVertices));
        mesh::generateCube(edgeVertices, vertexArray, indexArray);

        // Create planet
        {
            std::cout << "Starting noise generation..." << std::endl;
            std::chrono::steady_clock::time_point loadStart = std::chrono::steady_clock::now();
            mesh::planet(vertexArray, 8);
            std::chrono::steady_clock::time_point loadEnd = std::chrono::steady_clock::now();
            float loadTime = std::chrono::duration_cast<std::chrono::microseconds>(loadEnd - loadStart).count() / 1000000.0f;
            std::cout << "Done.  Process took " << loadTime << " seconds." << std::endl;
        }
        mesh::fixNormals(vertexArray, indexArray);

        // Add model to display
        Model model = Model(&vertexArray, &indexArray, &prog);
        display.addModel(&model);

        // Start display
        display.start();

        return 0;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
}
