
#include "display/display.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void resizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Display::Display(char const *title, int width, int height)
{
    // Initialise GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    if ((window = glfwCreateWindow(width, height, title, nullptr, nullptr)) == NULL)
    {
        glfwTerminate();
        throw std::exception("glfwCreateWindow failed.");
    }
    glfwMakeContextCurrent(window);

    // Load OpenGL functions with GLAD
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == NULL)
    {
        glfwTerminate();
        throw std::exception("gladLoadGLLoader failed.");
    }

    // Size OpenGL viewport to window and setup resize callback
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
}

Display::~Display()
{
    glfwTerminate();
}

bool Display::shouldClose() const
{
    return glfwWindowShouldClose(window);
}

void Display::frame()
{
    // Poll events
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Render
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Display image
    glfwSwapBuffers(window);
    glfwPollEvents();
}
