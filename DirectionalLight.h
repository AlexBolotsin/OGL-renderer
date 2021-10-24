#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight();
    DirectionalLight(glm::vec2 shadowSize, glm::vec3 col,
        GLfloat aintensity, GLfloat dIntensity,
        glm::vec3 dir);
    ~DirectionalLight();

    void UseLight(GLuint ambientIntensityUniform, GLuint ambientColorUniform,
        GLuint diffuseIntensityLocation, GLuint directionLocation) const ;

    glm::mat4 CalculateLightTransform() const;

private:
    glm::vec3 direction;
};

