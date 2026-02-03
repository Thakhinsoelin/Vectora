#version 330 core 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
out vec2 ScreenUV;
uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
//	gl_Position = vec4(aPos.x + mouseOffset.x, aPos.y + mouseOffset.y, aPos.z, 1.0);
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoord = aTexCoord;
	ScreenUV = aPos.xy;
}