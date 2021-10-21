#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight(glm::vec3 col,
        GLfloat aintensity, GLfloat dIntensity,
        glm::vec3 dir);
    ~DirectionalLight();

    void UseLight(GLuint ambientIntensityUniform, GLuint ambientColorUniform,
        GLuint diffuseIntensityLocation, GLfloat directionLocation) const ;

private:
    glm::vec3 direction;
};

