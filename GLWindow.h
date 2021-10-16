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

    void SetKey(int key, bool state) { keys[key] = state; }
    const bool* GetKeys() const { return keys; }

    GLfloat GetXChange() { GLfloat val = xChange; xChange = 0.f; return val; }
    GLfloat GetYChange() { GLfloat val = yChange; yChange = 0.f; return val; }

private:
    GLFWwindow* mainWindow;

    GLint width, height;

    bool keys[1024];
    GLfloat lastX;
    GLfloat lastY;
    GLfloat xChange;
    GLfloat yChange;
    bool mouseFIrstMoved;

    void CreateCallbacks();
    static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    static void HandleMouse(GLFWwindow* window, double xPos, double yPos);
};

