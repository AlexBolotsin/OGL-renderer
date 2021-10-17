#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>

#include <GL/glew.h>
class Shader
{
public:
    Shader();
    ~Shader();

    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    void CreateFromFiles(const char* vertexShader, const char* fragmentShader);

    std::string ReadFile(const char* vertexCode);

    GLuint GetProjectionUniform() const { return uniformProjection; }
    GLuint GetModelUniform() const { return uniformModel; }
    GLuint GetViewUniform() const { return uniformView; }
    GLuint GetAmbientIntensityLocation() const { return uniformAmbientIntensity; }
    GLuint GetAmbientColorLocation() const { return uniformAmbientColor; }
    GLuint GetDiffuseIntensityUniform() const { return uniformDiffuseIntensity; }
    GLuint GetDirectionUniform() const { return uniformDirection; }
    GLuint GetSpecularIntensityUniform() const { return uniformSpecularEntensity; }
    GLuint GetShininessUniform() const { return uniformShininess; }
    GLuint GetEyePositionUniform() const { return uniformEyePosition; }

    void UseShader();
    void ClearShader();

private:
    GLuint shaderID,
        uniformProjection,
        uniformModel,
        uniformView,
        uniformAmbientIntensity,
        uniformAmbientColor,
        uniformDiffuseIntensity,
        uniformDirection,
        uniformEyePosition,
        uniformSpecularEntensity,
        uniformShininess;

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(const char* shaderCode, GLenum shaderType);

};

