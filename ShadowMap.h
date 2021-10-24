#pragma once

#include <string>
#include <gl/glew.h>

class ShadowMap
{
public:
    ShadowMap() {}
    ~ShadowMap();

    virtual bool Init(GLuint width, GLuint height);
    virtual void Write();
    virtual void Read(GLenum textureUnit);

    GLuint GetShadowWidth() const { return shadowWidth; }
    GLuint GetShadowHeight() const { return shadowHeight; }


protected:
    GLuint FBO, shadowMap;
    GLuint shadowWidth = 0, shadowHeight = 0;
};

