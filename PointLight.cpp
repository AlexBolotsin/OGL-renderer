#include "PointLight.h"

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aintensity, GLfloat dIntensity,
    GLfloat xPos, GLfloat yPos, GLfloat zPos,
    GLfloat con, GLfloat lin, GLfloat exp) : Light(red, green, blue, aintensity, dIntensity)
{
    position = glm::vec3(xPos, yPos, zPos);
    constant = con;
    linear = lin;
    exponent = exp;
}

PointLight::~PointLight()
{
}

void PointLight::UseLight(GLuint ambientIntensityUniform, GLuint ambientColorUniform, GLuint diffuseIntensityLocation, GLfloat positionLocation, GLfloat constantUniform, GLfloat linearUniform, GLfloat exponentUniform) const
{
    glUniform1f(ambientIntensityUniform, ambientIntensity);
    glUniform3f(ambientColorUniform, color.r, color.g, color.b);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantUniform, constant);
    glUniform1f(linearUniform, linear);
    glUniform1f(exponentUniform, exponent);
}
