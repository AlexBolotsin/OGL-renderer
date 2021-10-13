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

    void UseShader();
    void ClearShader();

private:
    GLuint shaderID, uniformProjection, uniformModel;

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(const char* shaderCode, GLenum shaderType);

};

