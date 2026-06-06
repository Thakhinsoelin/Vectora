#include "vpch.h"
#include "Model.h"
#include <iostream>

namespace Vectora {
    
    Model::Model(const std::string& path, bool Tpose)
    {
        LoadModel(path, Tpose);
    }

    void Model::LoadModel(const std::string& path, bool Tpose)
    {
        Assimp::Importer importer;
        const aiScene* scene = nullptr;
        if (Tpose == true)
        {
            scene = importer.ReadFile(path, aiProcess_Triangulate | 
                                            aiProcess_FlipUVs | 
                                            aiProcess_CalcTangentSpace |
                                            aiProcess_PreTransformVertices);
        } else 
        {
            scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        }

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }

        m_Directory = path.substr(0, path.find_last_of('/'));

        // 1. Process all materials in the file first
        ProcessMaterials(scene);

        // 2. Process all geometry nodes
        ProcessNode(scene->mRootNode, scene);
    }

    void Model::ProcessMaterials(const aiScene* scene)
    {
        m_Materials.reserve(scene->mNumMaterials);

        for (unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            aiMaterial* aiMat = scene->mMaterials[i];
            Material material;

            // Extract Diffuse Maps
            if (aiMat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                aiString str;
                aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
                material.DiffuseMap = LoadTexture(m_Directory + "/" + str.C_Str());
            }

            // Extract Specular Maps
            if (aiMat->GetTextureCount(aiTextureType_SPECULAR) > 0) {
                aiString str;
                aiMat->GetTexture(aiTextureType_SPECULAR, 0, &str);
                material.SpecularMap = LoadTexture(m_Directory + "/" + str.C_Str());
            }

            if (aiMat->GetTextureCount(aiTextureType_NORMALS) > 0) {
                aiString str; aiMat->GetTexture(aiTextureType_NORMALS, 0, &str);
                material.NormalMap = LoadTexture(m_Directory + "/" + str.C_Str());
            }
            else if (aiMat->GetTextureCount(aiTextureType_HEIGHT) > 0) {
                aiString str; aiMat->GetTexture(aiTextureType_HEIGHT, 0, &str);
                material.NormalMap = LoadTexture(m_Directory + "/" + str.C_Str());
            }

            if (aiMat->GetTextureCount(aiTextureType_EMISSIVE) > 0)
            {
                aiString str;
                aiMat->GetTexture(aiTextureType_EMISSIVE, 0, &str);
                material.EmissionMap = LoadTexture(m_Directory + "/" + str.C_Str());
            }

            m_Materials.push_back(material);
        }
    }

    void Model::ProcessNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.push_back(ProcessMesh(mesh, scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<ModelVertex> vertices;
        std::vector<uint32_t> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            ModelVertex vertex;
            SetVertexBoneDataToDefault(vertex);
            vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

            if (mesh->HasNormals())
                vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
            else
                vertex.Normal = { 0.0f, 0.0f, 1.0f };

            if (mesh->HasTangentsAndBitangents()) {
                vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
                vertex.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
            }
            else {
                vertex.Tangent = { 0.0f, 0.0f, 0.0f };
                vertex.Bitangent = { 0.0f, 0.0f, 0.0f };
            }

            if (mesh->mTextureCoords[0])
                vertex.TexCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
            else
                vertex.TexCoords = { 0.0f, 0.0f };

            vertices.push_back(vertex);
        }
        ExtractBoneWeightForVertices(vertices, mesh, scene);

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // Return pure geometry attached to its natural material index
        return Mesh(vertices, indices, mesh->mMaterialIndex);
    }

    Ref<Texture2D> Model::LoadTexture(const std::string& path)
    {
        if (m_TextureCache.find(path) != m_TextureCache.end())
            return m_TextureCache[path]; // Return existing to save GPU memory!

        Ref<Texture2D> texture = Texture2D::Create(path);
        m_TextureCache[path] = texture;
        return texture;
    }
}