
#include <glad/glad.h> // Must be included before glfw3

#include "render/display.hpp"// Includes glfw3

#include "utility/debug.hpp"
#include"model/model.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

int const SAMPLES = 16; // Change in shader

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

    // Create framebuffer
    glGenFramebuffers(1, &backbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, backbuffer);

    // Backbuffer depth texture
    glGenRenderbuffers(1, &backbufferDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, backbufferDepth);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, SAMPLES, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, backbufferDepth);

    debug::zeroThrow(glCheckFramebufferStatus(GL_FRAMEBUFFER)==GL_FRAMEBUFFER_COMPLETE, "Not complete 1");

    // Backbuffer colour texture
    glGenTextures(1, &backbufferColour);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, backbufferColour);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, SAMPLES, GL_RGB, width, height, GL_TRUE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, backbufferColour, 0);

    debug::zeroThrow(glCheckFramebufferStatus(GL_FRAMEBUFFER)==GL_FRAMEBUFFER_COMPLETE, "Not complete 2");

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
    static glm::mat4 const view = glm::lookAt( glm::vec3(0.0f, 1.0f, 1.7f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) );
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 1000.0f);

    // Input
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Render scene onto buffer
    glBindFramebuffer(GL_FRAMEBUFFER, backbuffer);
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.4f, 0.7f, 0.9f, 1.0f);
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
    
    // Set uniforms for recreating world-space coordinates
    post->use();
    post->setUniformMatrix4fv("view_inverse", glm::inverse(view));
    post->setUniformMatrix4fv("projection_inverse", glm::inverse(projection));

    // Textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, backbufferColour);
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
}

void Display::resizeCallback(GLFWwindow *window, int width, int height)
{
    Display *display = (Display *)glfwGetWindowUserPointer(window);

    display->aspect = (float)width / height;
    display->windowWidth = width, display->windowHeight = height;

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, display->backbufferColour);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, SAMPLES, GL_RGB, width, height, GL_TRUE);
    glBindRenderbuffer(GL_RENDERBUFFER, display->backbufferDepth);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, SAMPLES, GL_DEPTH24_STENCIL8, width, height);
}
