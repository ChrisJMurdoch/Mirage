
#include <glad/glad.h> // Must be included before glfw3

#include "render/display.h"// Includes glfw3

#include "utility/debug.h"
#include"model/model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Display::Display(int width, int height, const char *title)
{
    // Initialise GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);
    
    // Sync size
    windowWidth = width, windowHeight = height;
    aspect = (float)width / height;
    windowSizeSynced = true;

    // Create window
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    debug::nullThrow(window, "GLFW window-creation exception");
    glfwSetWindowSizeLimits(window, 200, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwMakeContextCurrent(window);

    // Load OpenGL
    debug::zeroThrow( gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "GLAD OpenGL-loading exception" );
    glViewport(0, 0, width, height);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    std::cout << "+Display" << std::endl;
}

void Display::addModel(Model *model)
{
    models.push_back(model);
}

void Display::start()
{
    while (!glfwWindowShouldClose(window))
    {
        // Sync OpenGL
        if (!windowSizeSynced)
        {
            windowSizeSynced = true;
            glViewport(0, 0, windowWidth, windowHeight);
        }

        // Input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Clear
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render models
        for (Model *model : models)
        {
            static const glm::mat4 IDENTITY = glm::mat4(1.0f);
            glm::mat4 view = glm::translate(IDENTITY, glm::vec3(0.0f, 0.0f, -1.5f));
            static const float FOV = glm::radians(100.0f), NEAR = 0.1f, FAR = 100.0f;
            glm::mat4 projection = glm::perspective(FOV, aspect, NEAR, FAR);
            model->render(view, projection);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

Display::~Display()
{
    glfwTerminate();
    std::cout << "~Display" << std::endl;
}

void Display::resizeCallback(GLFWwindow *window, int width, int height)
{
    Display *display = (Display *)glfwGetWindowUserPointer(window);
    display->aspect = (float)width / height;
    display->windowWidth = width, display->windowHeight = height;
    display->windowSizeSynced = false;
}
