
#include <glad\glad.h> // Must be included before glfw3
#define display_p // Use private display header
#include "render\display.h"// Includes glfw3

#include"render\program.h"
#include"render\model.h"

#include <iostream>
#include <exception>

const char* vertexSource =
    "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n";

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

void display::create(int width, int height, const char *title)
{
    // Initialise GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        throw std::exception("GLFW window-creation exception");
    }
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwMakeContextCurrent(window);

    // Load OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        throw std::exception("GLAD OpenGL-loading exception");
    }

    // Create shader program
    Program prog = Program();
    prog.addShader(vertexSource, Program::Shader::VERTEX);
    prog.addShader(fragmentSource, Program::Shader::FRAGMENT);
    prog.link();

    // Create model
    Model model = Model(vertices, sizeof(vertices), indices, sizeof(indices), &prog);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        getInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw
        model.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void display::resizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void display::getInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
