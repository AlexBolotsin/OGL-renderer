#pragma once

#include "Light.h"
#include <vector>

class PointLight : public Light
{
public:
    PointLight() : Light() {}
    PointLight(glm::vec2 shadowSize, glm::vec3 color,
        glm::vec2 planes,
        GLfloat aIntensity, GLfloat dIntensity,
        glm::vec3 pos,
        GLfloat con, GLfloat lin, GLfloat exp);
    ~PointLight();

    void UseLight(GLuint ambientIntensityUniform, GLuint ambientColorUniform,
        GLuint diffuseIntensityLocation, GLuint positionLocation,
        GLuint constantUniform, GLuint linearUniform, GLuint exponentUniform) const;

    std::vector<glm::mat4> CalculateLightTransform() const;

    GLfloat GetFarPlane() const { return farPlane; }
    glm::vec3 GetPosition() const { return position; }
protected:
    glm::vec3 position;

    GLfloat constant, linear, exponent;

    GLfloat farPlane;
};

