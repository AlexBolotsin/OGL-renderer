#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 col,
    GLfloat aIntensity, GLfloat dIntensity,
    glm::vec3 dir) : Light(col, aIntensity, dIntensity)
{
    direction = dir;

}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::UseLight(GLuint ambientIntensityUniform, GLuint ambientColorUniform, GLuint diffuseIntensityLocation, GLfloat directionLocation) const
{
    glUniform1f(ambientIntensityUniform, ambientIntensity);
    glUniform3f(ambientColorUniform, color.r, color.g, color.b);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);

}
