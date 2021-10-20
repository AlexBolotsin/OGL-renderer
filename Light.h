#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aintensity, GLfloat dIntensity);
    ~Light();

protected:
    glm::vec3 color;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;

};

