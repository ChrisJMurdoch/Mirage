
#pragma once

#include "model/instance.h"

#include <GLFW/glfw3.h>

#include <list>

class Display
{
private:
    bool windowSizeSynced;
    int windowWidth, windowHeight;
    float aspect;
    GLFWwindow *window;
    std::list<Instance const *> instances;
public:
    Display(int width, int height, const char *title);
    void addInstance(Instance const * instance);
    void render();
    bool shouldClose();
    ~Display();
private:
    static void resizeCallback(GLFWwindow *window, int width, int height);
};
