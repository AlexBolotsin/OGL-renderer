#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
    GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity) : Light(red, blue, green, aIntensity, dIntensity)
{
    direction = glm::vec3(xDir, yDir, zDir);

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
