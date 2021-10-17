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
#include "Light.h"


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

bool direction = true;
float triOffset = 0.0f;
float triMamOffset = 0.7f;
float triIncrement = 0.005f;

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

        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));


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
        1.0f, -1.0f, 0.6f,      1.f, 0.f,       0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,       0.5f, 1.f,      0.0f, 0.0f, 0.0f
    };

    CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh* obj = new Mesh();
    obj->CreateMesh(vertices, indices, 32, 12);
    meshStorage.push_back(obj);
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

    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);
    Texture brickTex("assets/brickwork.png");
    brickTex.LoadTexture();
    Texture dirtTex("assets/dirt.png");
    dirtTex.LoadTexture();
    Light mainLight(1.f, 1.f, 1.f, 1.f,
        2.0f, -1.0f, -2.f, 0.3f);

    Material shinyMat(1.f, 32);
    Material dullMat(0.3f, 4);

    glm::mat4 projection = glm::perspective(45.f, (GLfloat)WIDTH/HEIGHT, 0.1f, 100.f);

    float curAngle = 0.f;
    bool sizeDirection = true;
    float curSize = 0.4f;
    const float sizeIncrement = 0.001f;

    while (!mainWindow.ShouldClose())
    {
        GLfloat now = (float)glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        camera.KeysControl(mainWindow.GetKeys(), deltaTime);
        camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

        triOffset += direction ? triIncrement : -triIncrement;

        if (abs(triOffset) >= triMamOffset)
            direction = !direction;

        curAngle += 0.1f;
        curAngle = normalize(curAngle, 0.f, 360.f);

        //curSize += sizeDirection ? sizeIncrement : -sizeIncrement;
        if (curSize > 0.8f || curSize < 0.3f)
        {
            sizeDirection = !sizeDirection;
            curSize = glm::clamp(curSize, 0.3f, 0.8f);
        }

        glClearColor(0.0, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Shader* shader = shaderList[0];
        shader->UseShader();

        glUniformMatrix4fv(shader->GetProjectionUniform(), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(shader->GetViewUniform(), 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
        glm::vec3 camPos = camera.GetCameraPosition();
        glUniform3f(shader->GetEyePositionUniform(), camPos.x, camPos.y, camPos.z);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.f, 0.f, -2.5f));
        //model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.f, 1.f, 0.0f));
        //model = glm::scale(model, glm::vec3(curSize, curSize, curSize));

        glUniformMatrix4fv(shader->GetModelUniform(), 1, GL_FALSE, glm::value_ptr(model));
        brickTex.UseTexture();
        shinyMat.UseMaterial(shader->GetSpecularIntensityUniform(), shader->GetShininessUniform());
        mainLight.UseLight(shader->GetAmbientIntensityLocation(), shader->GetAmbientColorLocation(),
            shader->GetDiffuseIntensityUniform(), shader->GetDirectionUniform());

        for (Mesh* mesh : meshStorage)
        {
            mesh->RenderMesh();
        }

        mainWindow.SwapBuffers();
    }

    return 0;
}

