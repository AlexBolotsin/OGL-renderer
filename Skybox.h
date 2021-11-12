#pragma once

#include <vector>

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

class Skybox
{
    Mesh* skyMesh;
    Shader* skyShader;

    GLuint textureID;
    GLuint uniformProjection, uniformView;
public:
    Skybox() {};
    Skybox(std::vector<std::string> faceLocations);
    ~Skybox();

    void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
};

