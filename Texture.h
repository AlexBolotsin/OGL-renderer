#pragma once

#include <GL/glew.h>
#include <gl/gl.h>

class Texture
{
public:
    Texture(const char* _filePath);
    ~Texture();

    void LoadTexture();
    void UseTexture();
    void ClearTexture();

private:
    GLuint textureID;
    int width, height, bitDepth;

    const char* filePath;
};

