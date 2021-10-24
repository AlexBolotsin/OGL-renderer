#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShadowMap.h"

class Light
{
public:
    Light() {}
    Light(glm::vec2 shadowSize, glm::vec3 col, GLfloat aIntensity, GLfloat dIntensity);
    ~Light();

    ShadowMap* GetShadowMap() const { return shadowMap; }

protected:
    glm::vec3 color;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;

    glm::mat4 lightProj;

    ShadowMap* shadowMap;
};

