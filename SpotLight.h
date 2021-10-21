#pragma once
#include "PointLight.h"
class SpotLight :
    public PointLight
{
public:
    SpotLight() {}
    SpotLight(glm::vec3 col, GLfloat aIntensity, GLfloat dIntensity,
        glm::vec3 pos, glm::vec3 dir,
        GLfloat con, GLfloat lin, GLfloat exp, GLfloat edg);
    ~SpotLight();

    void UseLight(GLuint ambientIntensityUniform, GLuint ambientColorUniform,
        GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionUniform,
        GLuint constantUniform, GLuint linearUniform, GLuint exponentUniform, GLuint edgeLocation) const;

    void SetFlash(glm::vec3 pos, glm::vec3 dir);
private:
    glm::vec3 direction;

    GLfloat edge, procEdge;
};

