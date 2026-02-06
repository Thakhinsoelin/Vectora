#version 330 core
layout (location = 0) in vec2 a_Position;

out vec4 a_Color;

void main()
{
    gl_Position = vec4(a_Position, 0.0, 1.0);
    a_Color = gl_Position;
}
