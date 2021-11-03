#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader()
{
    shaderID = 0;
    uniformModel = uniformProjection = uniformView = 0;

    pointLightCount = 0;
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

void Shader::CreateFromFiles(const char* vertexShader, const char* geometryShader, const char* fragmentShader)
{
    CompileShader(ReadFile(vertexShader).c_str(), ReadFile(geometryShader).c_str(), ReadFile(fragmentShader).c_str());
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

    CompileProgram();
}

void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
    shaderID = glCreateProgram();

    if (!shaderID)
    {
        printf("Error creating shader program");
        return;
    }

    AddShader(vertexCode, GL_VERTEX_SHADER);
    AddShader(geometryCode, GL_GEOMETRY_SHADER);
    AddShader(fragmentCode, GL_FRAGMENT_SHADER);

    CompileProgram();
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

void Shader::Validate()
{
    GLchar  eLog[1024];
    GLint result;

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error validating program: %s\n", eLog);
        return;
    }
}

void Shader::CompileProgram()
{
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

    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformView = glGetUniformLocation(shaderID, "view");
    uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
    uniformSpecularEntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");

    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
        uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
    }

    uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
        uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
    }

    uniformTexture = glGetUniformLocation(shaderID, "theTexture");
    uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
    uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

    uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
    uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

    for (int i = 0; i < 6; i++) {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
        uniformLightMatrices[i] = glGetUniformLocation(shaderID, locBuff);
    }

    for (int i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++) {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i);
        uniformOmniShadowMap[i].shadowMap = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i);
        uniformOmniShadowMap[i].farPlane = glGetUniformLocation(shaderID, locBuff);
    }
}

void Shader::SetDirectionalLight(const DirectionalLight& dLight)
{
    dLight.UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor,
        uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(const PointLight* plights, unsigned int count, unsigned int textureUnit, unsigned int offset)
{
    if (count > MAX_POINT_LIGHTS) count = MAX_POINT_LIGHTS;

    glUniform1i(uniformPointLightCount, count);

    for (int i = 0l; i < count; i++)
    {
        plights[i].UseLight(uniformPointLight[i].uniformAmbientIntensity,
            uniformPointLight[i].uniformColor,
            uniformPointLight[i].uniformDiffuseIntensity,
            uniformPointLight[i].uniformPosition,
            uniformPointLight[i].uniformConstant,
            uniformPointLight[i].uniformLinear,
            uniformPointLight[i].uniformExponent);

        plights[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
        glUniform1f(uniformOmniShadowMap[i + offset].farPlane, plights[i].GetFarPlane());
    }
}

void Shader::SetSpotLights(const SpotLight* slights, unsigned int count, unsigned int textureUnit, unsigned int offset)
{
    if (count > MAX_SPOT_LIGHTS) count = MAX_SPOT_LIGHTS;

    glUniform1i(uniformSpotLightCount, count);

    for (int i = 0l; i < count; i++)
    {
        slights[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity,
            uniformSpotLight[i].uniformColor,
            uniformSpotLight[i].uniformDiffuseIntensity,
            uniformSpotLight[i].uniformPosition,
            uniformSpotLight[i].uniformDirection,
            uniformSpotLight[i].uniformConstant,
            uniformSpotLight[i].uniformLinear,
            uniformSpotLight[i].uniformExponent,
            uniformSpotLight[i].uniformEdge);

        slights[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);
        glUniform1f(uniformOmniShadowMap[i + offset].farPlane, slights[i].GetFarPlane());
    }
}

void Shader::SetTexture(GLuint texture)
{
    glUniform1i(uniformTexture, texture);
}

void Shader::SetDirectionalShadowMap(GLuint textuteUnit)
{
    glUniform1i(uniformDirectionalShadowMap, textuteUnit);
}

void Shader::SetDirectionalLightTransform(const glm::mat4& transform)
{
    glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(transform));
}

void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices)
{
    for (int i = 0; i < 6; i++)
    {
        glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
    }
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
