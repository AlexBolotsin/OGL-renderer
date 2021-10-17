#include "Shader.h"

Shader::Shader()
{
    shaderID = 0;
    uniformModel = uniformProjection = uniformView = 0;
}

Shader::~Shader()
{
    ClearShader();
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexShader, const char* fragmentShader)
{
    CompileShader(ReadFile(vertexShader).c_str(), ReadFile(fragmentShader).c_str());
}

std::string Shader::ReadFile(const char* file)
{
    std::fstream shader(file, std::ios::in);
    std::string code;


    if (!shader.is_open())
    {
        printf("Failed to open shader file %s", file);
        return code;
    }

    std::string line = "";
    while (!shader.eof())
    {
        std::getline(shader, line);
        code.append(line + '\n');
    }

    shader.close();
    return code;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    shaderID = glCreateProgram();

    if (!shaderID)
    {
        printf("Error creating shader program");
        return;
    }

    AddShader(vertexCode, GL_VERTEX_SHADER);
    AddShader(fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { '\0' };

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error linking program: %s\n", eLog);
        return;
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error validating program: %s\n", eLog);
        return;
    }

    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformView = glGetUniformLocation(shaderID, "view");
    uniformAmbientColor = glGetUniformLocation(shaderID, "dirLight.color");
    uniformAmbientIntensity = glGetUniformLocation(shaderID, "dirLight.ambientIntensity");
    uniformDirection = glGetUniformLocation(shaderID, "dirLight.direction");
    uniformDiffuseIntensity = glGetUniformLocation(shaderID, "dirLight.diffuseIntensity");
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
    uniformSpecularEntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
}

void Shader::AddShader(const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = (GLint)strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { '\0' };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling %d shader: %s\n", theShader, eLog);
        return;
    }

    glAttachShader(shaderID, theShader);
}

void Shader::UseShader()
{
    glUseProgram(shaderID);
}

void Shader::ClearShader()
{
    if (shaderID)
    {
        glDeleteShader(shaderID);
        shaderID = 0;
    }

    uniformModel = uniformProjection = 0;
}
