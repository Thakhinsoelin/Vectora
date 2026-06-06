#include "Mesh.h"

namespace Vectora 
{
    Mesh::Mesh(const std::vector<ModelVertex>& vertices, const std::vector<uint32_t>& indices, uint32_t materialIndex)
        : m_MaterialIndex(materialIndex)
    {
        m_VertexArray = VertexArray::Create();

        m_VertexBuffer = VertexBuffer::Create((float*)vertices.data(), vertices.size() * sizeof(ModelVertex));
        m_VertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal"   },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float3, "a_Tangent"  },
            { ShaderDataType::Float3, "a_Bitangent"},
            { ShaderDataType::Int4, "a_BoneIDs"},
            { ShaderDataType::Float4, "a_Weights"}
        });
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);

        m_IndexBuffer = IndexBuffer::Create((uint32_t*)indices.data(), indices.size());
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    }
}