#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aintensity,
        GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);
    ~DirectionalLight();

    void UseLight(GLuint ambientIntensityUniform, GLuint ambientColorUniform,
        GLuint diffuseIntensityLocation, GLfloat directionLocation) const ;

private:
    glm::vec3 direction;
};

