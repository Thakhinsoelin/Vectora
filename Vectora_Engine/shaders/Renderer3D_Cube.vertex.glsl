#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_WorldPosition;

void main()
{
    v_TexCoord = a_TexCoord;
    
    // Transform normal to world space (handles scaling and rotation safely)
    v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
    
    // Get world coordinates of vertex for lighting calculations
    v_WorldPosition = vec3(u_Transform * vec4(a_Position, 1.0));
    
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}