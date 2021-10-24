#include "Light.h"

Light::Light(glm::vec2 shadowSize, glm::vec3 col, GLfloat aIntensity, GLfloat dIntensity)
{
    color = col;
    ambientIntensity = aIntensity;
    diffuseIntensity = dIntensity;
    shadowMap = new ShadowMap();
    shadowMap->Init(shadowSize.x, shadowSize.y);
}

Light::~Light()
{
    //delete shadowMap;
}
