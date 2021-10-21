#include "PointLight.h"

PointLight::PointLight(glm::vec3 color, GLfloat aintensity, GLfloat dIntensity,
    glm::vec3 pos,
    GLfloat con, GLfloat lin, GLfloat exp) : Light(color, aintensity, dIntensity)
{
    position = pos;
    constant = con;
    linear = lin;
    exponent = exp;
}

PointLight::~PointLight()
{
}

void PointLight::UseLight(GLuint ambientIntensityUniform, GLuint ambientColorUniform, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint constantUniform, GLuint linearUniform, GLuint exponentUniform) const
{
    glUniform1f(ambientIntensityUniform, ambientIntensity);
    glUniform3f(ambientColorUniform, color.r, color.g, color.b);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantUniform, constant);
    glUniform1f(linearUniform, linear);
    glUniform1f(exponentUniform, exponent);
}
