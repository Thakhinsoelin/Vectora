#pragma once

#include <vpch.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "Shader.h"

#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

#define MAX_BONE_INFLUENCE 4

namespace Vectora
{
	struct Material {
        Ref<Shader> ShaderOverride = nullptr; // Leave null to use Renderer3D's default
        Ref<Texture2D> DiffuseMap = nullptr;
        Ref<Texture2D> SpecularMap = nullptr;

        Ref<Texture2D> NormalMap = nullptr;
        Ref<Texture2D> EmissionMap = nullptr;

        glm::vec4 AlbedoColor = glm::vec4(1.0f); // Fallback color if no texture exists
        float Shininess = 32.0f;
	};

	struct ModelVertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;

        glm::vec3 Tangent;
        glm::vec3 Bitangent;

        //bone indexes which will influence this vertex
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        //weights from each bone
        float m_Weights[MAX_BONE_INFLUENCE];

	};

	class Mesh {
    public:
        Mesh() = default;
        Mesh(const std::vector<ModelVertex>& vertices, const std::vector<uint32_t>& indices, uint32_t materialIndex);

        const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
        uint32_t GetIndexCount() const { return m_IndexBuffer->GetCount(); }
        uint32_t GetMaterialIndex() const { return m_MaterialIndex; }

    private:
        Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
        uint32_t m_MaterialIndex;
    };
}