#include "Skybox.h"
#include "Common.h"

Skybox::Skybox(std::vector<std::string> faceLocations)
{
    skyShader = new Shader();
    skyShader->CreateFromFiles("shaders/skybox.vert", "shaders/skybox.frag");

    uniformProjection = skyShader->GetProjectionUniform();
    uniformView = skyShader->GetViewUniform();

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, bitDepth;

    for (int i = 0; i < 6; i++)
    {
        unsigned char* texData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, 0);
        if (!texData)
        {
            printf("Failed to load %s\n", faceLocations[i].c_str());
            continue;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
        stbi_image_free(texData);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int skyboxIndices[] = {
        0,1,2,
        2,1,3,

        2,3,5,
        5,3,7,

        5,7,4,
        4,7,6,

        4,6,0,
        0,6,1,

        4,0,5,
        5,0,2,

        1,6,3,
        3,6,7
    };

    float skyboxVertices[] = {
    -1.0f, 1.0f, -1.0f,         0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,        0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, -1.0f,          0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, -1.0f,         0.0f, 0.0f,     0.0f, 0.0f, 0.0f,

    -1.0f, 1.0f, 1.0f,      0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,       0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, 1.0f,     0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f,      0.0f, 0.0f,     0.0f, 0.0f, 0.0f
    };


    skyMesh = new Mesh();
    skyMesh->CreateMesh(skyboxVertices, skyboxIndices, 64, 36);
}

Skybox::~Skybox()
{
}

void Skybox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    viewMatrix = glm::mat4(glm::mat3(viewMatrix));


    glDepthMask(GL_FALSE);

    skyShader->UseShader();
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    skyShader->Validate();

    skyMesh->RenderMesh();

    glDepthMask(GL_TRUE);
}
