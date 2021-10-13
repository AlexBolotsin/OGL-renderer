#include <vector>

#include "Shader.h"
#include "Mesh.h"

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

const int WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.f;

bool direction = true;
float triOffset = 0.0f;
float triMamOffset = 0.7f;
float triIncrement = 0.005f;

const char shaderVS[] = "shaders/shader.vert";

const char shaderFS[] = "shaders/shader.frag";

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(shaderVS, shaderFS);
    shaderList.push_back(shader1);
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
        -1.0f, -1.0f, 0.0f,
        0.f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    Mesh* obj = new Mesh();
    obj->CreateMesh(vertices, indices, 12, 12);
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

    glm::mat4 projection = glm::perspective(90.f, (GLfloat)WIDTH/HEIGHT, 0.1f, 100.f);

    float curAngle = 0.f;
    bool sizeDirection = true;
    float curSize = 0.4f;
    const float sizeIncrement = 0.001f;

    while (!mainWindow.ShouldClose())
    {
        glfwPollEvents();

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

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.f, triOffset, -2.5f));
        model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.f, 1.f, 0.0f));
        model = glm::scale(model, glm::vec3(curSize, curSize, curSize));

        glUniformMatrix4fv(shader->GetModelUniform(), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(shader->GetProjectionUniform(), 1, GL_FALSE, glm::value_ptr(projection));

        for (Mesh* mesh : meshStorage)
        {
            mesh->RenderMesh();
        }

        mainWindow.SwapBuffers();
    }

    return 0;
}

