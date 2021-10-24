#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>

#include <GL/glew.h>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

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
    GLuint GetAmbientIntensityLocation() const { return uniformDirectionalLight.uniformAmbientIntensity; }
    GLuint GetAmbientColorLocation() const { return uniformDirectionalLight.uniformColor; }
    GLuint GetDiffuseIntensityUniform() const { return uniformDirectionalLight.uniformDiffuseIntensity; }
    GLuint GetDirectionUniform() const { return uniformDirectionalLight.uniformDirection; }
    GLuint GetSpecularIntensityUniform() const { return uniformSpecularEntensity; }
    GLuint GetShininessUniform() const { return uniformShininess; }
    GLuint GetEyePositionUniform() const { return uniformEyePosition; }

    void SetDirectionalLight(const DirectionalLight& dLight);
    void SetPointLights(const PointLight* plights, unsigned int count);
    void SetSpotLights(const SpotLight* slights, unsigned int count);
    void SetTexture(GLuint texture);
    void SetDirectionalShadowMap(GLuint textuteUnit);
    void SetDirectionalLightTransform(const glm::mat4& transform);

    void UseShader();
    void ClearShader();

private:
    int pointLightCount;
    int spotLightCount;

    GLuint shaderID,
        uniformProjection,
        uniformModel,
        uniformView,
        uniformEyePosition,
        uniformSpecularEntensity,
        uniformShininess,
        uniformTexture,
        uniformDirectionalLightTransform,
        uniformDirectionalShadowMap;

    struct {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;

        GLuint uniformDirection;
    } uniformDirectionalLight;

    GLuint uniformPointLightCount;

    struct {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;

        GLuint uniformPosition;

        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
    } uniformPointLight[MAX_POINT_LIGHTS];

    GLuint uniformSpotLightCount;

    struct {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;

        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;

        GLuint uniformDirection;
        GLuint uniformEdge;
    } uniformSpotLight[MAX_SPOT_LIGHTS];

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(const char* shaderCode, GLenum shaderType);
};

