#include "PointLight.h"
#include "OmniShadowMap.h"
#include <glm\gtc\matrix_transform.hpp>

PointLight::PointLight(glm::vec2 shadowSize, glm::vec3 color,
    glm::vec2 planes, GLfloat aintensity, GLfloat dIntensity,
    glm::vec3 pos,
    GLfloat con, GLfloat lin, GLfloat exp) : Light(shadowSize, color, aintensity, dIntensity)
{
    position = pos;
    constant = con;
    linear = lin;
    exponent = exp;
    farPlane = planes.y;

    float aspect = shadowSize.x / shadowSize.y;
    lightProj = glm::perspective(glm::radians(90.f), aspect, planes.x, planes.y);

    shadowMap = new OmniShadowMap();
    shadowMap->Init(shadowSize.x, shadowSize.y);
}

PointLight::~PointLight()
{
}

void PointLight::UseLight(GLuint ambientIntensityUniform, GLuint ambientColorUniform, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint constantUniform, GLuint linearUniform, GLuint exponentUniform) const
{
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    glUniform1f(ambientIntensityUniform, ambientIntensity);
    glUniform3f(ambientColorUniform, color.r, color.g, color.b);
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantUniform, constant);
    glUniform1f(linearUniform, linear);
    glUniform1f(exponentUniform, exponent);
}

std::vector<glm::mat4> PointLight::CalculateLightTransform() const
{
    std::vector<glm::mat4> lightMatrices;
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
    return lightMatrices;
}
