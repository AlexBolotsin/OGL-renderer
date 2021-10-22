#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::LoadModel(const std::string& filename)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if (!scene)
    {
        printf("Model (%s) failed to load %s\n", filename.c_str(), importer.GetErrorString());
        return;
    }

    LoadNode(scene->mRootNode, scene);
    LoadMaterial(scene);
}

void Model::RenderModel() const
{
    for (size_t i = 0; i < meshList.size(); i++)
    {
        unsigned int materialIndex = meshToTex[i];

        if (materialIndex < textureList.size() && textureList[materialIndex])
        {
            textureList[materialIndex]->UseTexture();
        }
        meshList[i]->RenderMesh();
    }
}

void Model::ClearModel()
{
    for (size_t i = 0; i < meshList.size(); i++)
    {
        if (meshList[i])
        {
            delete meshList[i];
        }
    }
    meshList.clear();

    for (size_t i = 0; i < textureList.size(); i++)
    {
        if (textureList[i])
        {
            delete textureList[i];
        }
    }
    textureList.clear();
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
    for (int i = 0; i < node->mNumMeshes; ++i)
    {
        LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }

    for (int i = 0; i < node->mNumChildren; i++)
    {
        LoadNode(node->mChildren[i], scene);
    }
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
        if (mesh->mTextureCoords[0])
        {
            vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
        }
        else {
            vertices.insert(vertices.end(), { 0.f, 0.f });
        }
        vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh* newMesh = new Mesh();
    newMesh->CreateMesh(vertices.data(), indices.data(), vertices.size(), indices.size());
    meshList.push_back(newMesh);
    meshToTex.push_back(mesh->mMaterialIndex);

}

void Model::LoadMaterial(const aiScene* scene)
{
    textureList.resize(scene->mNumMaterials);

    for (size_t i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* material = scene->mMaterials[i];

        textureList[i] = nullptr;

        if (material->GetTextureCount(aiTextureType_DIFFUSE))
        {
            aiString path;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            {
                int idx = std::string(path.data).rfind('\\');
                std::string filename = std::string(path.data).substr(idx + 1);

                std::string texPath = std::string("Textures/") + filename;

                textureList[i] = new Texture(texPath.c_str());

                if (!textureList[i]->LoadTexture())
                {
                    printf("Failed to load texture %s\n", texPath.c_str());
                    delete textureList[i];
                    textureList[i] = nullptr;
                }
            }
        }
        if (!textureList[i])
        {
            textureList[i] = new Texture("Textures/plain.png");
            textureList[i]->LoadTexture();
        }
    }
}
