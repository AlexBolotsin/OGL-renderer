#include "Light.h"

Light::Light(glm::vec3 col, GLfloat aIntensity, GLfloat dIntensity)
{
    color = col;
    ambientIntensity = aIntensity;
    diffuseIntensity = dIntensity;
}

Light::~Light()
{

}
