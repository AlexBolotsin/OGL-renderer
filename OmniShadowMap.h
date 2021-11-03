#pragma once

#include "ShadowMap.h"

class OmniShadowMap : public ShadowMap
{
public:
    OmniShadowMap();
    ~OmniShadowMap();

    virtual bool Init(GLuint width, GLuint height);
    virtual void Write();
    virtual void Read(GLenum textureUnit);
private:

};

