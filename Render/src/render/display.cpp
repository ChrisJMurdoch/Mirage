
#include <glad\glad.h> // Must be included before glfw3
#define display_p // Use private display header
#include "render\display.h"// Includes glfw3

#include <iostream>
#include <exception>

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

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        getInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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
