
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
    
    // Set initial dimensions
    windowWidth = width, windowHeight = height;
    aspect = (float)width / height;

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Backbuffer colour texture
    glGenTextures(1, &backbufferColour);
    glBindTexture(GL_TEXTURE_2D, backbufferColour);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    
    // Backbuffer depth texture
    glGenTextures(1, &backbufferDepth);
    glBindTexture(GL_TEXTURE_2D, backbufferDepth);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    
    // Create backbuffer
    glGenFramebuffers(1, &backbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, backbuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, backbufferColour, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, backbufferDepth, 0);
    debug::zeroThrow(glCheckFramebufferStatus(GL_FRAMEBUFFER)==GL_FRAMEBUFFER_COMPLETE, "Failed to create backbuffer");

    // Generate screen quad
    float quadVertices[] ={
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    unsigned int screenQuadVBO;
    glGenVertexArrays(1, &screenQuadVAO);
    glGenBuffers(1, &screenQuadVBO);
    glBindVertexArray(screenQuadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    std::cout << "+Display" << std::endl;
}

void Display::addPostProgram(Program *prog)
{
    post = prog;

    // Set texture locations
    post->setUniform1i("colour", 0);
    post->setUniform1i("depth", 1);
}

void Display::addInstance(Instance const * instance)
{
    instances.push_back(instance);
}

void Display::render()
{
    debug::nullThrow(post, "Must call addPostProgram() before render()");

    // Transform matrices
    static glm::mat4 const view = glm::lookAt( glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) );
    static glm::mat4 const view_inverse = glm::inverse(view);
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.1f, 100.0f);
    glm::mat4 projection_inverse = glm::inverse(projection);

    // Input
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Render scene onto buffer
    glBindFramebuffer(GL_FRAMEBUFFER, backbuffer);
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (Instance const *const instance : instances)
    {
        instance->render(view, projection);
    }
    
    // Post-process from buffer to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set uniforms
    post->use();
    post->setUniform3fv("planet_centre", glm::vec3(0, 0, 0));
    post->setUniformMatrix4fv("view_inverse", view_inverse);
    post->setUniformMatrix4fv("projection_inverse", projection_inverse);

    // Textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, backbufferColour);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, backbufferDepth);
    glBindVertexArray(screenQuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // Show
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Display::shouldClose()
{
    return glfwWindowShouldClose(window);
}

Display::~Display()
{
    glDeleteFramebuffers(1, &backbuffer);
    glfwTerminate();
    std::cout << "~Display" << std::endl;
}

void Display::resizeCallback(GLFWwindow *window, int width, int height)
{
    Display *display = (Display *)glfwGetWindowUserPointer(window);
    display->aspect = (float)width / height;
    display->windowWidth = width, display->windowHeight = height;

    glBindTexture(GL_TEXTURE_2D, display->backbufferColour);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, display->backbufferDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
}
