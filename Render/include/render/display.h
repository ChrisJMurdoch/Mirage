
#pragma once

#include <GLFW\glfw3.h>

#include "render\model.h"

#include <list>

class Display
{
private:
    GLFWwindow* window;
    std::list<Model*> models;
public:
    Display(int width, int height, const char* title);
    void addModel(Model* model);
    void start();
    ~Display();
};
