#pragma once
#include "Renderer/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include <map>

#define MAX_BONE_WEIGHTS MAX_BONE_INFLUENCE

namespace Vectora {
    struct BoneInfo
    {
        /*id is index in finalBoneMatrices*/
        int id;

        /*offset matrix transforms vertex from model space to bone space*/
        glm::mat4 offset;

    };

    class Model {
    public:
        Model(const std::string& path, bool Tpose = false);

        const std::vector<Mesh>& GetMeshes() const { return m_Meshes; }
        const std::vector<Material>& GetMaterials() const { return m_Materials; }

    private:
        void LoadModel(const std::string& path, bool Tpose = false);
        void ProcessMaterials(const aiScene* scene);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        Ref<Texture2D> LoadTexture(const std::string& path);

    private:
        std::vector<Mesh> m_Meshes;
        std::vector<Material> m_Materials;
        std::string m_Directory;

        // Caches textures by path so we don't load the same file twice
        std::unordered_map<std::string, Ref<Texture2D>> m_TextureCache;

    private:
        glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from) {
            glm::mat4 to;
            to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
            to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
            to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
            to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
            return to;
        }

    
        std::map<std::string, BoneInfo> m_BoneInfoMap; //
        int m_BoneCounter = 0;

    public:
        auto& GetBoneInfoMap() { return m_BoneInfoMap; }
        int& GetBoneCount() { return m_BoneCounter; }
        void SetVertexBoneDataToDefault(ModelVertex& vertex)
        {
            for (int i = 0; i < MAX_BONE_WEIGHTS; i++)
            {
                vertex.m_BoneIDs[i] = -1;
                vertex.m_Weights[i] = 0.0f;
            }
        }

        void SetVertexBoneData(ModelVertex& vertex, int boneID, float weight)
        {
            for (int i = 0; i < MAX_BONE_WEIGHTS; ++i)
            {
                if (vertex.m_BoneIDs[i] < 0)
                {
                    vertex.m_Weights[i] = weight;
                    vertex.m_BoneIDs[i] = boneID;
                    break;
                }
            }
        }

        void ExtractBoneWeightForVertices(std::vector<ModelVertex>& vertices, aiMesh* mesh, const aiScene* scene)
        {
            for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
            {
                int boneID = -1;
                std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
                if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
                {
                    BoneInfo newBoneInfo;
                    newBoneInfo.id = m_BoneCounter;
                    newBoneInfo.offset = ConvertMatrixToGLMFormat(
                        mesh->mBones[boneIndex]->mOffsetMatrix);
                    m_BoneInfoMap[boneName] = newBoneInfo;
                    boneID = m_BoneCounter;
                    m_BoneCounter++;
                }
                else
                {
                    boneID = m_BoneInfoMap[boneName].id;
                }
                assert(boneID != -1);
                auto weights = mesh->mBones[boneIndex]->mWeights;
                int numWeights = mesh->mBones[boneIndex]->mNumWeights;

                for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
                {
                    int vertexId = weights[weightIndex].mVertexId;
                    float weight = weights[weightIndex].mWeight;
                    assert(vertexId <= vertices.size());
                    SetVertexBoneData(vertices[vertexId], boneID, weight);
                }
            }
        }
    };
}