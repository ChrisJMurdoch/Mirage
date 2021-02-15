
#include <glad\glad.h> // Must be included before glfw3
#define display_p // Use private display header
#include "render\display.h"// Includes glfw3

#include"render\program.h"
#include"render\model.h"
#include "utility\debug.h"

#include <iostream>
#include <exception>

void resizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Display::Display(int width, int height, const char *title)
{
    // Initialise GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    // Create window
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    debug::nullThrow(window, "GLFW window-creation exception");
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwMakeContextCurrent(window);

    // Load OpenGL
    debug::zeroThrow( gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "GLAD OpenGL-loading exception" );

    std::cout << "+Display." << std::endl;
}

void Display::addModel(Model *model)
{
    models.push_back(model);
}

void Display::start()
{
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render all models
        for (Model* model : models)
        {
            model->render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

Display::~Display()
{
    glfwTerminate();
    std::cout << "~Display." << std::endl;
}
