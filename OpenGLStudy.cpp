#define STB_IMAGE_IMPLEMENTATION

#include <vector>

#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLWindow.h"


float Q_rsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(long*)&y;                       // evil floating point bit level hacking
    i = 0x5f3759df - (i >> 1);               // what the fuck? 
    y = *(float*)&i;
    y = y * (threehalfs - (x2 * y * y));   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
}

std::vector<Mesh*> meshStorage;
std::vector<Shader*> shaderList;

const int WIDTH = 1366, HEIGHT = 768;
const float toRadians = 3.14159265f / 180.f;

GLfloat deltaTime = 0.f;
GLfloat lastTime = 0.f;

const char shaderVS[] = "shaders/shader.vert";

const char shaderFS[] = "shaders/shader.frag";

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(shaderVS, shaderFS);
    shaderList.push_back(shader1);
}

void CalcAverageNormals(unsigned int* indeces, unsigned int indiceCount,
    GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
    for (int i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indeces[i] * vLength;
        unsigned int in1 = indeces[i + 1] * vLength;
        unsigned int in2 = indeces[i + 2] * vLength;

        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        const glm::vec3 normal = glm::normalize(glm::cross(v1, v2));


        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;

        vertices[in0] += normal.x;
        vertices[in0 + 1] += normal.y;
        vertices[in0 + 2] += normal.z;

        vertices[in1] += normal.x;
        vertices[in1 + 1] += normal.y;
        vertices[in1 + 2] += normal.z;

        vertices[in2] += normal.x;
        vertices[in2 + 1] += normal.y;
        vertices[in2 + 2] += normal.z;
    }

    for (int i = 0; i < verticeCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x;
        vertices[nOffset + 1] = vec.y;
        vertices[nOffset + 2] = vec.z;
    }
}

void CreateObjects()
{
    GLuint indices[] = {
        0, 3, 1,
        1, 3, 2,
        2 ,3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        -1.0f, -1.0f, -0.6f,     0.f, 0.f,       0.0f, 0.0f, 0.0f,
        0.f, -1.0f, 1.0f,       0.5f, 0.f,      0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -0.6f,      1.f, 0.5f,       0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,       0.5f, 1.f,      0.0f, 0.0f, 0.0f
    };

    GLuint floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] = {
        -10.f, 0.f, -10.f,     0.f, 0.f,   0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, -10.f,  10.0f, 0.f,   0.0f, -1.0f, 0.0f,
        -10.f, 0.0f, 10.f,    0.f, 10.f,   0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, 10.f,    10.f, 10.f,   0.0f, -1.0f, 0.0f
    };

    CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

    {
        Mesh* obj = new Mesh();
        obj->CreateMesh(vertices, indices, 32, 12);
        meshStorage.push_back(obj);
    }
    {
        Mesh* obj = new Mesh();
        obj->CreateMesh(vertices, indices, 32, 12);
        meshStorage.push_back(obj);
    }

    Mesh* floor = new Mesh();
    floor->CreateMesh(floorVertices, floorIndices, 32, 6);
    meshStorage.push_back(floor);
}

double normalize(const double value, const double start, const double end)
{
    const double width = end - start;   // 
    const double offsetValue = value - start;   // value relative to 0

    return (offsetValue - (floor(offsetValue / width) * width)) + start;
    // + start to reset back to start of original range
}

float normalize(const float value, const float start, const float end)
{
    const float width = end - start;   // 
    const float offsetValue = value - start;   // value relative to 0

    return (offsetValue - (floor(offsetValue / width) * width)) + start;
    // + start to reset back to start of original range
}

int main()
{
    GLWindow mainWindow(WIDTH, HEIGHT);
    if (mainWindow.Initialize())
    {
        return 1;
    }
    
    CreateObjects();
    CreateShaders();

    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);
    Texture brickTex("assets/brickwork.png");
    brickTex.LoadTexture();
    Texture dirtTex("assets/dirt.png");
    dirtTex.LoadTexture();
    Texture blankTex("assets/blank.png");
    blankTex.LoadTexture();
    DirectionalLight mainLight(glm::vec3(1.f, 1.f, 1.f), 0.0f,
        0.0f, glm::vec3(0.0f, -1.f, 0.3f));
    PointLight pointLights[MAX_POINT_LIGHTS] = {
        {
            glm::vec3(0.f, 1.f, 0.f),
            0.0f, 0.0f,
            glm::vec3(-2.f, 0.f, 0.f),
            0.3f, 0.2f, 0.1f
        }
        /*{
            0.f, 0.f, 1.f,
            0.1f, 0.1f,
            2.f, 0.f, 0.f,
            0.3f, 0.2f, 0.1f
        },
        {
            0.f, 1.f, 0.f,
            0.1f, 0.1f,
            -4.f, 2.f, 0.f,
            0.3f, 0.1f, 0.1f
        }*/
    };
    int pointLightsCount = 0;
    SpotLight spotLight[MAX_SPOT_LIGHTS] = {
        {
            glm::vec3(1.f, 1.f, 1.f),
            0.0f, 1.0f,
            glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(0.f, -1.f, 0.f),
            1.0f, 0.0f, 0.0f,
            10.f
        },
        {
            glm::vec3(1.f, 1.f, 1.f),
            0.0f, 2.0f,
            glm::vec3(0.f, 1.5f, 0.f),
            glm::vec3(-100.f, -1.f, 0.f),
            1.0f, 0.0f, 0.0f,
            50.f
        }
    };
    int spotLightsCount = 2;


    Material shinyMat(1.f, 32*32);
    Material dullMat(0.3f, 4);

    glm::mat4 projection = glm::perspective(45.f, (GLfloat)mainWindow.GetBufferWidth()/mainWindow.GetBufferHeight(), 0.1f, 100.f);

    while (!mainWindow.ShouldClose())
    {
        GLfloat now = (float)glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        camera.KeysControl(mainWindow.GetKeys(), deltaTime);
        camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Shader* shader = shaderList[0];
        shader->UseShader();

        glUniformMatrix4fv(shader->GetProjectionUniform(), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(shader->GetViewUniform(), 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
        glm::vec3 camPos = camera.GetCameraPosition();
        glUniform3f(shader->GetEyePositionUniform(), camPos.x, camPos.y, camPos.z);

        //shader->SetDirectionalLight(mainLight);
        //shader->SetPointLights(pointLights, pointLightsCount);
        shader->SetSpotLights(spotLight, spotLightsCount);

        glm::vec3 lowerLigh = camera.GetCameraPosition() - glm::vec3(-0.1f, -0.3f, 0.f);
        spotLight[0].SetFlash(lowerLigh, camera.GetCameraDirection());

        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.f, 0.f, -2.5f));
            glUniformMatrix4fv(shader->GetModelUniform(), 1, GL_FALSE, glm::value_ptr(model));
            brickTex.UseTexture();
            shinyMat.UseMaterial(shader->GetSpecularIntensityUniform(), shader->GetShininessUniform());
            meshStorage[0]->RenderMesh();
        }
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.f, 0.f, -5.f));
            glUniformMatrix4fv(shader->GetModelUniform(), 1, GL_FALSE, glm::value_ptr(model));
            dirtTex.UseTexture();
            dullMat.UseMaterial(shader->GetSpecularIntensityUniform(), shader->GetShininessUniform());
            meshStorage[0]->RenderMesh();
        }
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(0.f, -2.f, 0.f));
            glUniformMatrix4fv(shader->GetModelUniform(), 1, GL_FALSE, glm::value_ptr(model));
            dirtTex.UseTexture();
            dullMat.UseMaterial(shader->GetSpecularIntensityUniform(), shader->GetShininessUniform());
            meshStorage[2]->RenderMesh();
        }
        glUseProgram(0);
        mainWindow.SwapBuffers();
    }

    return 0;
}

