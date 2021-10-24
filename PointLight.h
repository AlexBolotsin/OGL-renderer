#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
    PointLight() : Light() {}
    PointLight(glm::vec2 shadowSize, glm::vec3 color,
        GLfloat aIntensity, GLfloat dIntensity,
        glm::vec3 pos,
        GLfloat con, GLfloat lin, GLfloat exp);
    ~PointLight();

    void UseLight(GLuint ambientIntensityUniform, GLuint ambientColorUniform,
        GLuint diffuseIntensityLocation, GLuint positionLocation,
        GLuint constantUniform, GLuint linearUniform, GLuint exponentUniform) const;
protected:
    glm::vec3 position;

    GLfloat constant, linear, exponent;
};

