#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
    Light(glm::vec3 col, GLfloat aintensity, GLfloat dIntensity);
    ~Light();

protected:
    glm::vec3 color;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;

};

