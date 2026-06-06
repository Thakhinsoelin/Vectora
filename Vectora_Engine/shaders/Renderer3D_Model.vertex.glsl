#version 330 core

// 1. Vertex Attributes matching your VertexBuffer layout
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;
layout(location = 5) in ivec4 a_BoneIDs;
layout(location = 6) in vec4 a_Weights;

// 2. Uniforms pushed from Renderer3D::DrawModel
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

const int MAX_BONES = 100;
uniform mat4 u_FinalBonesMatrices[MAX_BONES];

// 3. Data to pass down to the Fragment Shader
out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TexCoord;
out mat3 v_TBN; // Pass the matrix to the fragment shader

void main()
{
// --- 1. Compute Bone Transformation Matrix ---
    mat4 BoneTransform = u_FinalBonesMatrices[a_BoneIDs[0]] * a_Weights[0];
    BoneTransform     += u_FinalBonesMatrices[a_BoneIDs[1]] * a_Weights[1];
    BoneTransform     += u_FinalBonesMatrices[a_BoneIDs[2]] * a_Weights[2];
    BoneTransform     += u_FinalBonesMatrices[a_BoneIDs[3]] * a_Weights[3];

    // --- 2. Apply Bone Transformation to Position & Normal ---
    // This shifts the vertex from "Bind Pose" space into "Animated Model" space
    vec4 localPosition = BoneTransform * vec4(a_Position, 1.0);
    vec3 localNormal   = mat3(BoneTransform) * a_Normal;
    vec3 localTangent  = mat3(BoneTransform) * a_Tangent;
    vec3 localBitangent = mat3(BoneTransform) * a_Bitangent;

    // Calculate the physical location of the vertex in the world
    v_FragPos = vec3(u_Transform * localPosition);
    
    // We update the normal matrix to include the BoneTransform, 
    // ensuring normals deform correctly with the mesh
    mat3 normalMatrix = mat3(transpose(inverse(u_Transform * BoneTransform)));
    v_Normal = normalMatrix * localNormal;
    
    // Pass UVs straight through
    v_TexCoord = a_TexCoord;

    // Create the TBN Matrix using the normal matrix to handle scaling/rotations
    vec3 T = normalize(normalMatrix * localTangent);
    vec3 B = normalize(normalMatrix * localBitangent);
    vec3 N = normalize(normalMatrix * localNormal);
    v_TBN = mat3(T, B, N);
    
    // Final screen-space position
    gl_Position = u_ViewProjection * vec4(v_FragPos, 1.0);
}