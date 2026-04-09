#version 330 core
			
layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

// Inputs from Vertex Shader
in vec3 v_Position;
in vec4 v_Color;

flat in int v_EntityID;


void main()
{
	o_Color = v_Color;
    o_EntityID = v_EntityID;
}