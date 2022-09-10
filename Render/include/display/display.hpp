
#pragma once

class GLFWwindow;

class Display
{
private:
    GLFWwindow *window;

public:
    Display(char const *title, int width, int height);
    ~Display();

    Display(Display const &other) = delete;
    Display &operator=(Display const &other) = delete;

    bool shouldClose() const;
    void frame();
};
