#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 offset;
out vec4 a_Color;

void main()
{
    vec2 pos = aPos + offset;
    gl_Position = vec4(pos, 0.0, 1.0);
    a_Color = gl_Position;
}
