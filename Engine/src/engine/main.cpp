
#include "engine\main.h"

#include "generate\mesh.h"
#include "render\display.h"
#include "utility\io.h"
#include "render\attributeSpecifier.h"

#include <iostream>
#include <exception>

int main()
{
    try
    {
        // Create display
        Display display = Display(1000, 600, "Redshift");


        // Create Mesh
        AttributeSpecifier spec = AttributeSpecifier(6, 0, AttributeSpecifier::NONE, 3);
        Mesh mesh = Mesh(10, &spec);
        mesh.morph([](glm::vec3 &vector) { vector = glm::normalize(vector); });

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
        
        // Create model
        Model model = Model(&mesh, &prog);
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
