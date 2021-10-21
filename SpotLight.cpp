#include "SpotLight.h"
#include "glm/gtc/type_ptr.hpp"

SpotLight::SpotLight(glm::vec3 col, GLfloat aintensity, GLfloat dIntensity,
    glm::vec3 pos, glm::vec3 dir, GLfloat con, GLfloat lin, GLfloat exp, GLfloat edg) : PointLight(col, aintensity, dIntensity,
        pos, con, lin, exp)
{
    direction = glm::normalize(dir);
    edge = edg;
    procEdge = cosf(glm::radians(edge));
}

SpotLight::~SpotLight()
{
}

void SpotLight::UseLight(GLuint ambientIntensityUniform,
    GLuint ambientColorUniform,
    GLuint diffuseIntensityLocation,
    GLuint positionLocation,
    GLuint directionUniform,
    GLuint constantUniform,
    GLuint linearUniform,
    GLuint exponentUniform,
    GLuint edgeLocation) const
{
    PointLight::UseLight(ambientIntensityUniform,
        ambientColorUniform,
        diffuseIntensityLocation,
        positionLocation,
        constantUniform,
        linearUniform,
        exponentUniform);

     glUniform3fv(directionUniform, 1, glm::value_ptr(direction));
    //glUniform3f(directionUniform, direction.x, direction.y, direction.z);
    glUniform1f(edgeLocation, procEdge);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
    position = pos;
    direction = dir;
}
