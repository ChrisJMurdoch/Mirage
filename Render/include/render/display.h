
#pragma once

#include "model/model.h"

#include <GLFW/glfw3.h>

#include <list>

class Display
{
private:
    bool windowSizeSynced;
    int windowWidth, windowHeight;
    float aspect;
    GLFWwindow *window;
    std::list<Model *> models;
public:
    Display(int width, int height, const char *title);
    void addModel(Model *model);
    void start();
    ~Display();
private:
    static void resizeCallback(GLFWwindow *window, int width, int height);
};
