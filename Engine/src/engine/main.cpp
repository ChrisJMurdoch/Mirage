
#include "engine\main.h"

#include "render\display.h"
#include "utility\io.h"

#include <iostream>
#include <exception>

const char* fragmentSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

float vertices[] = {
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
};
unsigned int indices[] = {
    0, 1, 2,
    0, 1, 3
};

int main()
{
    try
    {
        // Create display
        Display display = Display(1000, 600, "Redshift");

        // Create shader program
        Program prog = Program();

        std::string *vertexSource = io::read("shaders\\shader.vert");
        prog.addShader(vertexSource->c_str(), Program::Shader::VERTEX);
        delete vertexSource;

        std::string *fragmentSource = io::read("shaders\\shader.frag");
        prog.addShader(fragmentSource->c_str(), Program::Shader::FRAGMENT);
        delete fragmentSource;

        prog.link();

        // Add model
        Model model = Model(vertices, sizeof(vertices), indices, sizeof(indices), &prog);
        display.addModel(&model);

        // Start render loop
        display.start();

        return 0;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
}
