#pragma once
#include <GL/glew.h>
class Material
{
public:
    Material(GLfloat sIntensity, GLfloat shine);
    ~Material();

    void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation) const;

private:
    GLfloat specularIntensity;
    GLfloat shininess;
};

