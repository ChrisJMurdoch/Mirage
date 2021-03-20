
#pragma once

#include "model/instance.hpp"

#include <GLFW/glfw3.h>

#include <list>

class Display
{
private:
    int windowWidth, windowHeight;
    float aspect;
    GLFWwindow *window;
    Program *post = nullptr;
    unsigned int backbuffer, backbufferColour, backbufferDepth, screenQuadVAO;
    std::list<Instance const *> instances;
public:
    Display(int width, int height, const char *title);
    void addPostProgram(Program *prog);
    void addInstance(Instance const * instance);
    void render();
    bool shouldClose();
    ~Display();
private:
    static void resizeCallback(GLFWwindow *window, int width, int height);
};
