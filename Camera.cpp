#include "Camera.h"
#include <GLFW\glfw3.h>

Camera::Camera(
    glm::vec3 _position,
    glm::vec3 _up,
    GLfloat _yaw,
    GLfloat _pitch,
    GLfloat _movementSpeed,
    GLfloat _turnSpeed
)
{
    position = _position;
    worldUp = _up;
    yaw = _yaw;
    pitch = _pitch;
    front = glm::vec3(0.f, 0.f, -1.f);

    movementSpeed = _movementSpeed;
    turnSpeed = _turnSpeed;

    Update();
}

Camera::~Camera()
{
}

void Camera::KeysControl(const bool* keys, float deltaTime)
{
    auto velocity = movementSpeed * deltaTime;

    if (keys[GLFW_KEY_W])
    {
        position += front * velocity;
    }
    if (keys[GLFW_KEY_A])
    {
        position -= right * velocity;
    }
    if (keys[GLFW_KEY_S])
    {
        position -= front * velocity;
    }
    if (keys[GLFW_KEY_D])
    {
        position += right * velocity;
    }
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange)
{
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;

    pitch = pitch > 89.f ? 89.f : pitch;
    pitch = pitch < -89.f ? -89.f : pitch;

    Update();
}

glm::mat4 Camera::CalculateViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

void Camera::Update()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
