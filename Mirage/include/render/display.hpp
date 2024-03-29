
#pragma once

#include <unordered_set>

class GLFWwindow;
class Model;

class Display
{
public:
    int width, height;

private:
    GLFWwindow *window;
    std::unordered_set<Model const *> models;
    bool visible;

public:
    Display(char const *title, int width, int height);
    ~Display();
    Display(Display const &other) = delete;
    Display &operator=(Display const &other) = delete;

    bool shouldClose() const;
    void render();

    void registerModel(Model const *model);
    void deregisterModel(Model const *model);
};
