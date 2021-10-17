#include "Material.h"

Material::Material(GLfloat sIntensity, GLfloat shine)
{
    specularIntensity = sIntensity;
    shininess = shine;
}

Material::~Material()
{
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation) const
{
    glUniform1f(specularIntensityLocation, specularIntensity);
    glUniform1f(shininessLocation, shininess);
}
