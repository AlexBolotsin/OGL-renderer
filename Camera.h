#pragma once

#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Camera
{
public:
    Camera(
        glm::vec3 _position,
        glm::vec3 _up,
        GLfloat _yaw,
        GLfloat _pitch,
        GLfloat _movementSpeed,
        GLfloat _turnSpeed
    );
    ~Camera();

    void KeysControl(const bool* keys, float deltaTime);
    void MouseControl(GLfloat xChange, GLfloat yChange);

    glm::mat4 CalculateViewMatrix() const;

    glm::vec3 GetCameraPosition() const { return position; }

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat movementSpeed;
    GLfloat turnSpeed;

    void Update();
};

