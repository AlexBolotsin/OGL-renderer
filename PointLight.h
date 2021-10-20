#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
    PointLight() : Light(0.f, 0.f, 0.f, 0.f, 0.f) {}
    PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aintensity, GLfloat dIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos,
        GLfloat con, GLfloat lin, GLfloat exp);
    ~PointLight();

    void UseLight(GLuint ambientIntensityUniform, GLuint ambientColorUniform,
        GLuint diffuseIntensityLocation, GLfloat positionLocation,
        GLfloat constantUniform, GLfloat linearUniform, GLfloat exponentUniform) const;
private:
    glm::vec3 position;

    GLfloat constant, linear, exponent;
};

