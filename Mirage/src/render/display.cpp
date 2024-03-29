
#include "render/display.hpp"

#include "render/model.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <exception>

void resizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    Display *display = reinterpret_cast<Display *>(glfwGetWindowUserPointer(window));
    display->width = width;
    display->height = height;
}

Display::Display(char const *title, int width, int height) : width(width), height(height), visible(false)
{
    // Initialise GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

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

    // GL Options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    // Set user pointer for callbacks
    glfwSetWindowUserPointer(window, reinterpret_cast<void *>(this));

    // Size OpenGL viewport to window and setup resize callback
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
}

Display::~Display()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Display::shouldClose() const
{
    return glfwWindowShouldClose(window);
}

void Display::render()
{
    // Show window if hidden
    if (!visible)
    {
        visible=true;
        glfwShowWindow(window);
    }

    // Poll events
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // Clear viewport
    float backgroundBrightness = 0.05f;
    glClearColor(backgroundBrightness, backgroundBrightness, backgroundBrightness, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render each model
    for (Model const *model : models)
        model->draw();

    // Display image
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Display::registerModel(Model const *model)
{
    models.insert(model);
}

void Display::deregisterModel(Model const *model)
{
    models.erase(model);
}
