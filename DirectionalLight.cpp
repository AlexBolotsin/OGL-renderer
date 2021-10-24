#include "DirectionalLight.h"
#include <glm/gtc/matrix_transform.hpp>

DirectionalLight::DirectionalLight() : Light()
{
}

DirectionalLight::DirectionalLight(glm::vec2 shadowSize, glm::vec3 col,
    GLfloat aIntensity, GLfloat dIntensity,
    glm::vec3 dir) : Light(shadowSize, col, aIntensity, dIntensity)
{
    direction = dir;
    lightProj = glm::ortho(-20.f, 20.f, -20.f, 20.f, 0.1f, 100.f);
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::UseLight(GLuint ambientIntensityUniform, GLuint ambientColorUniform,
    GLuint diffuseIntensityLocation, GLuint directionLocation) const
{
    glUniform3f(ambientColorUniform, color.r, color.g, color.b);
    glUniform1f(ambientIntensityUniform, ambientIntensity);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);

}

glm::mat4 DirectionalLight::CalculateLightTransform() const
{
    return lightProj * glm::lookAt(-direction, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
}
