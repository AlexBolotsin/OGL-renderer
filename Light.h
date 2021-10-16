#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aintensity,
        GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);
    ~Light();

    void UseLight(GLuint ambientIntensityUniform, GLuint ambientColorUniform,
        GLuint diffuseIntensityLocation, GLfloat directionLocation);

private:
    glm::vec3 color;
    GLfloat ambientIntensity;

    glm::vec3 direction;
    GLfloat diffuseIntensity;

};

