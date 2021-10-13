#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLWindow
{
public:
    GLWindow(GLint windowWidth, GLint windowHeight);
    ~GLWindow();

    int Initialize();

    GLint GetBufferWidth() const;
    GLint GetBufferHeight() const;

    bool ShouldClose() const {
        return glfwWindowShouldClose(mainWindow);
    }

    void SwapBuffers() { glfwSwapBuffers(mainWindow); }
private:
    GLFWwindow* mainWindow;

    GLint width, height;
};

