    #include "vpch.h"
    #include "Core/Core.h"
    #include "Scene/SceneCamera.h"
    #include "Debug/Instrumentor.h"

    #include "Renderer/Renderer3D.h"
    #include "Renderer/VertexArray.h"
    #include "Renderer/Buffer.h"
    #include "Renderer/RenderCommand.h"

    #include "Renderer/Model.h"
    #include "Renderer/Texture.h"
    #include <glm/gtc/matrix_transform.hpp>

    #include "platforms/OpenGL/OpenGLShader.h"

    namespace Vectora {

	    struct Vertex3D {
		    glm::vec3 Position;
		    glm::vec3 Normal;
		    glm::vec2 TexCoords;
	    };

        Vertex3D cubeVerticies[] = {
            // Position-------------   // Normals-------------   // TexCoords---

            // Front Face (Z+)
            { {-0.5f, -0.5f,  0.5f},   { 0.0f,  0.0f,  1.0f},   {0.0f, 0.0f} }, // Bottom-Left
            { { 0.5f, -0.5f,  0.5f},   { 0.0f,  0.0f,  1.0f},   {1.0f, 0.0f} }, // Bottom-Right
            { { 0.5f,  0.5f,  0.5f},   { 0.0f,  0.0f,  1.0f},   {1.0f, 1.0f} }, // Top-Right
            { {-0.5f,  0.5f,  0.5f},   { 0.0f,  0.0f,  1.0f},   {0.0f, 1.0f} }, // Top-Left

            // Back Face (Z-)
            { { 0.5f, -0.5f, -0.5f},   { 0.0f,  0.0f, -1.0f},   {0.0f, 0.0f} }, // Bottom-Left
            { {-0.5f, -0.5f, -0.5f},   { 0.0f,  0.0f, -1.0f},   {1.0f, 0.0f} }, // Bottom-Right
            { {-0.5f,  0.5f, -0.5f},   { 0.0f,  0.0f, -1.0f},   {1.0f, 1.0f} }, // Top-Right
            { { 0.5f,  0.5f, -0.5f},   { 0.0f,  0.0f, -1.0f},   {0.0f, 1.0f} }, // Top-Left

            // Top Face (Y+)
            { {-0.5f,  0.5f,  0.5f},   { 0.0f,  1.0f,  0.0f},   {0.0f, 0.0f} }, // Bottom-Left
            { { 0.5f,  0.5f,  0.5f},   { 0.0f,  1.0f,  0.0f},   {1.0f, 0.0f} }, // Bottom-Right
            { { 0.5f,  0.5f, -0.5f},   { 0.0f,  1.0f,  0.0f},   {1.0f, 1.0f} }, // Top-Right
            { {-0.5f,  0.5f, -0.5f},   { 0.0f,  1.0f,  0.0f},   {0.0f, 1.0f} }, // Top-Left

            // Bottom Face (Y-)
            { {-0.5f, -0.5f, -0.5f},   { 0.0f, -1.0f,  0.0f},   {0.0f, 0.0f} }, // Bottom-Left
            { { 0.5f, -0.5f, -0.5f},   { 0.0f, -1.0f,  0.0f},   {1.0f, 0.0f} }, // Bottom-Right
            { { 0.5f, -0.5f,  0.5f},   { 0.0f, -1.0f,  0.0f},   {1.0f, 1.0f} }, // Top-Right
            { {-0.5f, -0.5f,  0.5f},   { 0.0f, -1.0f,  0.0f},   {0.0f, 1.0f} }, // Top-Left

            // Left Face (X-)
            { {-0.5f, -0.5f, -0.5f},   {-1.0f,  0.0f,  0.0f},   {0.0f, 0.0f} }, // Bottom-Left
            { {-0.5f, -0.5f,  0.5f},   {-1.0f,  0.0f,  0.0f},   {1.0f, 0.0f} }, // Bottom-Right
            { {-0.5f,  0.5f,  0.5f},   {-1.0f,  0.0f,  0.0f},   {1.0f, 1.0f} }, // Top-Right
            { {-0.5f,  0.5f, -0.5f},   {-1.0f,  0.0f,  0.0f},   {0.0f, 1.0f} }, // Top-Left

            // Right Face (X+)
            { { 0.5f, -0.5f,  0.5f},   { 1.0f,  0.0f,  0.0f},   {0.0f, 0.0f} }, // Bottom-Left
            { { 0.5f, -0.5f, -0.5f},   { 1.0f,  0.0f,  0.0f},   {1.0f, 0.0f} }, // Bottom-Right
            { { 0.5f,  0.5f, -0.5f},   { 1.0f,  0.0f,  0.0f},   {1.0f, 1.0f} }, // Top-Right
            { { 0.5f,  0.5f,  0.5f},   { 1.0f,  0.0f,  0.0f},   {0.0f, 1.0f} }  // Top-Left
        };

        uint32_t cubeIndices[] = {
            0,  1,  2,     2,  3,  0,  // Front Face
            4,  5,  6,     6,  7,  4,  // Back Face
            8,  9,  10,    10, 11, 8,  // Top Face
            12, 13, 14,    14, 15, 12, // Bottom Face
            16, 17, 18,    18, 19, 16, // Left Face
            20, 21, 22,    22, 23, 20  // Right Face
        };

	    struct Renderer3DData {
		    Ref<VertexArray> CubeVertexArray;
		    Ref<VertexBuffer> CubeVertexBuffer;
		    Ref<IndexBuffer> CubeIndexBuffer;
		    Ref<Shader> CubeShader;

            Ref<Shader> DefaultModelShader;
            Ref<Texture2D> WhiteTexture; // <--- ADD THIS HERE
            Ref<Texture2D> BlackTexture;         // <-- ADD
            Ref<Texture2D> DefaultNormalTexture; // <-- ADD

            Ref<Shader> AnimatedModelShader; // <-- New

            glm::mat4 ViewProjectionMatrix;
	    };

        static Renderer3DData s_3DData;

	    void Renderer3D::Init()
	    {
		    VE_PROFILE_FUNCTION();
		    s_3DData.CubeVertexArray = VertexArray::Create();
		    s_3DData.CubeVertexBuffer = VertexBuffer::Create(reinterpret_cast<float*>(cubeVerticies), sizeof(cubeVerticies));
            s_3DData.CubeVertexBuffer->SetLayout({
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float3, "a_Normal"   },
                { ShaderDataType::Float2, "a_TexCoord" }
                });
		    s_3DData.CubeVertexArray->AddVertexBuffer(s_3DData.CubeVertexBuffer);
		    s_3DData.CubeIndexBuffer = IndexBuffer::Create(cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t));
		    s_3DData.CubeVertexArray->SetIndexBuffer(s_3DData.CubeIndexBuffer);

		    s_3DData.CubeShader = Shader::Create("shaders/Renderer3D_Cube.vertex.glsl", "shaders/Renderer3D_Cube.fragment.glsl");
            s_3DData.DefaultModelShader = Shader::Create("shaders/Renderer3D_Model.vertex.glsl", "shaders/Renderer3D_Model.fragment.glsl");
            s_3DData.AnimatedModelShader = s_3DData.DefaultModelShader;//Shader::Create("shaders/Renderer3D_AnimatedModel.vertex.glsl", "shaders/Renderer3D_AnimatedModel.fragment.glsl");

            s_3DData.WhiteTexture = Texture2D::Create(1, 1);
            uint32_t whiteTextureData = 0xffffffff;
            s_3DData.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

            // 1. Create Opaque Black (for missing Emission maps so they don't glow)
            s_3DData.BlackTexture = Texture2D::Create(1, 1);
            unsigned char blackTextureData[] = { 0, 0, 0, 255 }; // R=0, G=0, B=0, A=255
            s_3DData.BlackTexture->SetData(blackTextureData, sizeof(blackTextureData));

            // 2. Create Default Normal Map (Flat surface facing directly out: vec3(0.5, 0.5, 1.0))
            s_3DData.DefaultNormalTexture = Texture2D::Create(1, 1);
            unsigned char normalTextureData[] = { 128, 128, 255, 255 }; // R=128, G=128, B=255, A=255
            s_3DData.DefaultNormalTexture->SetData(normalTextureData, sizeof(normalTextureData));


            s_3DData.DefaultModelShader->Bind();
            s_3DData.DefaultModelShader->setInt("u_DiffuseMap", 0);  // Tell shader Diffuse is at Slot 0
            s_3DData.DefaultModelShader->setInt("u_SpecularMap", 1); // Tell shader Specular is at Slot 1
            s_3DData.DefaultModelShader->setInt("u_NormalMap", 2);   // Slot 2
            s_3DData.DefaultModelShader->setInt("u_EmissionMap", 3); // Slot 3
	    }

        void Renderer3D::Shutdown()
        {
            // No manual new[] pointers used, smart references handle destruction!
        }
        void Renderer3D::BeginScene(const CameraC& camera, const glm::mat4& transform)
        {
            s_3DData.ViewProjectionMatrix = camera.GetProjection() * transform;
		    VE_PROFILE_FUNCTION();
		    s_3DData.CubeShader->Bind();
		    s_3DData.CubeShader->setMat4("u_ViewProjection", s_3DData.ViewProjectionMatrix);
        }

        void Renderer3D::BeginScene(const EditorCamera& camera)
        {
            s_3DData.ViewProjectionMatrix = camera.GetViewProjection();

            s_3DData.CubeShader->Bind();
            s_3DData.CubeShader->setMat4("u_ViewProjection", s_3DData.ViewProjectionMatrix);
        }

        void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
        {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                * glm::scale(glm::mat4(1.0f), size);

            DrawCube(transform, color);
        }

        void Renderer3D::DrawCube(const glm::mat4& transform, const glm::vec4& color)
        {
            s_3DData.CubeShader->Bind();
            s_3DData.CubeShader->setMat4("u_Transform", transform);
            s_3DData.CubeShader->setVec4("u_Color", color);

            s_3DData.CubeVertexArray->Bind();
            RenderCommand::DrawIndexed(s_3DData.CubeVertexArray, 36);
        }

        void Renderer3D::EndScene() {

        }
        // 1. Native Model Drawing (Supports multi-mesh / multi-material automatically)
        void Renderer3D::DrawModel(const glm::mat4& transform, const Ref<Model>& model)
        {
            const auto& meshes = model->GetMeshes();
            const auto& materials = model->GetMaterials();

            for (const auto& mesh : meshes)
            {
                // Fetch the specific material for this chunk of geometry
                const Material& mat = materials[mesh.GetMaterialIndex()];

                Ref<Shader> activeShader = mat.ShaderOverride ? mat.ShaderOverride : s_3DData.DefaultModelShader;
                activeShader->Bind();
                activeShader->setMat4("u_ViewProjection", s_3DData.ViewProjectionMatrix);
                activeShader->setMat4("u_Transform", transform);

                // Bind textures to fixed slots (0 = Diffuse, 1 = Specular)
                if (mat.DiffuseMap) mat.DiffuseMap->Bind(0);
                else s_3DData.WhiteTexture->Bind(0);

                if (mat.SpecularMap) mat.SpecularMap->Bind(1);
                else s_3DData.WhiteTexture->Bind(1); // Or bind a black/default specular map

                // Normals fallback to the Flat Normal Blue Texture
                if (mat.NormalMap) mat.NormalMap->Bind(2);
                else s_3DData.DefaultNormalTexture->Bind(2);

                // Emission falls back to Pitch Black (so it doesn't glow white!)
                if (mat.EmissionMap) mat.EmissionMap->Bind(3);
                else s_3DData.BlackTexture->Bind(3);

                mesh.GetVertexArray()->Bind();
                RenderCommand::DrawIndexed(mesh.GetVertexArray(), mesh.GetIndexCount());
            }
        }

        // 2. Override Model Drawing (Forces a single material across the entire model)
        void Renderer3D::DrawModel(const glm::mat4& transform, const Ref<Model>& model, const Material& overrideMaterial)
        {
            Ref<Shader> activeShader = overrideMaterial.ShaderOverride ? overrideMaterial.ShaderOverride : s_3DData.DefaultModelShader;

            activeShader->Bind();
            activeShader->setMat4("u_ViewProjection", s_3DData.ViewProjectionMatrix);
            activeShader->setMat4("u_Transform", transform);

            if (overrideMaterial.DiffuseMap) overrideMaterial.DiffuseMap->Bind(0);
            else s_3DData.WhiteTexture->Bind(0);

            if (overrideMaterial.SpecularMap) overrideMaterial.SpecularMap->Bind(1);
            else s_3DData.WhiteTexture->Bind(1);



            // Draw all geometry chunks using this single override state
            for (const auto& mesh : model->GetMeshes())
            {
                mesh.GetVertexArray()->Bind();
                RenderCommand::DrawIndexed(mesh.GetVertexArray(), mesh.GetIndexCount());
            }
        }

        void Renderer3D::DrawAnimatedModel(const glm::mat4& transform, const Ref<Model>& model, SkeAnimator& animator)
        {
            const auto& meshes = model->GetMeshes();
            const auto& materials = model->GetMaterials();

            // Bind the dedicated animated shader
            Ref<Shader> activeShader = s_3DData.AnimatedModelShader;
            activeShader->Bind();

            // 1. Upload the bone matrices to the shader
            auto& boneMatrices = animator.GetFinalBoneMatrices();
            for (size_t i = 0; i < boneMatrices.size(); i++)
            {
                // Use a string key like "u_FinalBonesMatrices[0]"
                activeShader->setMat4("u_FinalBonesMatrices[" + std::to_string(i) + "]", boneMatrices[i]);
            }

            // 2. Set standard view-projection and world space transform uniforms
            activeShader->setMat4("u_ViewProjection", s_3DData.ViewProjectionMatrix);
            activeShader->setMat4("u_Transform", transform);

            // 3. Loop through and draw each mesh component with its materials
            for (const auto& mesh : meshes)
            {
                // Fetch the specific material for this chunk of geometry
                const Material& mat = materials[mesh.GetMaterialIndex()];

                // Bind textures to fixed slots (0 = Diffuse, 1 = Specular, 2 = Normal, 3 = Emission)
                if (mat.DiffuseMap) mat.DiffuseMap->Bind(0);
                else s_3DData.WhiteTexture->Bind(0);

                if (mat.SpecularMap) mat.SpecularMap->Bind(1);
                else s_3DData.WhiteTexture->Bind(1);

                // Normals fallback to the Flat Normal Blue Texture
                if (mat.NormalMap) mat.NormalMap->Bind(2);
                else s_3DData.DefaultNormalTexture->Bind(2);

                // Emission falls back to Pitch Black (so missing maps don't glow)
                if (mat.EmissionMap) mat.EmissionMap->Bind(3);
                else s_3DData.BlackTexture->Bind(3);

                // Bind VAO and issue the raw index buffer draw call
                mesh.GetVertexArray()->Bind();
                RenderCommand::DrawIndexed(mesh.GetVertexArray(), mesh.GetIndexCount());
            }
        }
    }