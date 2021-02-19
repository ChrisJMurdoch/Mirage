
#pragma once

#include "model/model.h"

#include <GLFW/glfw3.h>

#include <list>

class Display
{
private:
    float aspect = 1;
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
