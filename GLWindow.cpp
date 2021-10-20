#include "GLWindow.h"

#include <memory>

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;
    memset(keys, false, 1024);
    mouseFIrstMoved = true;
    xChange = yChange = 0.f;
}

GLWindow::~GLWindow()
{
}

int GLWindow::Initialize()
{
    if (glfwInit() != GLFW_TRUE)
    {
        printf("GLFW init failed");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "Test window", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window create failed");
        glfwTerminate();
        return 1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // set main window context
    glfwMakeContextCurrent(mainWindow);

    CreateCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW init failed");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);
    return 0;
}

GLint GLWindow::GetBufferWidth() const
{
    return width;
}

GLint GLWindow::GetBufferHeight() const
{
    return height;
}

void GLWindow::CreateCallbacks()
{
    glfwSetKeyCallback(mainWindow, HandleKeys);
    glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

void GLWindow::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        theWindow->SetKey(key, action == GLFW_PRESS || action == GLFW_REPEAT ? true : false);
        printf("Keyboard key [%d] %s [%d]\n", key, action == GLFW_PRESS ? "pressed" : "release", action);
    }
}

void GLWindow::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
    GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

    if (theWindow->mouseFIrstMoved)
    {
        theWindow->lastX = (GLfloat)xPos;
        theWindow->lastY = (GLfloat)yPos;
        theWindow->mouseFIrstMoved = false;
    }

    theWindow->xChange = (GLfloat)xPos - theWindow->lastX;
    theWindow->yChange = -((GLfloat)yPos - theWindow->lastY);

    theWindow->lastX = (GLfloat)xPos;
    theWindow->lastY = (GLfloat)yPos;

    printf("Mouse changes are [%f] [%f]\n", theWindow->xChange, theWindow->yChange);
}
