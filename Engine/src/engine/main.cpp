
#include "engine/main.h"

#include "render/display.h"
#include "utility/io.h"
#include "generate/mesh.h"
#include "model/virtualVector.h"

#include <iostream>
#include <exception>

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
        const int edgeVertices = 100;
        VertexArray vertexArray(mesh::vertexCount(edgeVertices));
        IndexArray indexArray(mesh::indexCount(edgeVertices));
        mesh::generateCube(edgeVertices, &vertexArray, &indexArray);

        // Sphere
        mesh::morph(&vertexArray, [](VirtualVector vector) { vector.normalise(); });

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
