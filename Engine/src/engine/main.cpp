
#include "engine\main.h"

#include "render\display.h"
#include "utility\io.h"

#include <iostream>
#include <exception>

float vertices[] = {
    -1, -1, 0,   1, 0, 0,
     1, -1, 0,   0, 1, 0,
     1,  1, 0,   0, 0, 1,
    -1,  1, 0,   1, 0, 1,
};
unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0,
};

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
        
        // Create model
        Model model = Model(vertices, sizeof(vertices), indices, sizeof(indices), &prog);
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
