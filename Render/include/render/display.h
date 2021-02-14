
#pragma once

// Public header
namespace display
{
	void create(int width, int height, const char* title);
}

// Private header
#ifdef display_p

#include <GLFW\glfw3.h>

namespace display
{
    void resizeCallback(GLFWwindow* window, int width, int height);
    void getInput(GLFWwindow* window);
}

#endif
