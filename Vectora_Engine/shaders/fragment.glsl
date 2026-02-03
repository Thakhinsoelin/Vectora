#version 330 core
out vec4 FragColor;


in vec2 TexCoord;
in vec2 ScreenUV;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float rate;
uniform vec2 mousePos;
void main()
{
	vec2 screenUV = gl_FragCoord.xy / vec2(800.0, 600.0);
	vec4 texColor = texture(texture2, screenUV);
	float dist = distance(screenUV, mousePos);
	float radius = 0.1;
    float edgeSoftness = 0.05;
    float brightness = 1.0 - smoothstep(radius, radius + edgeSoftness, dist);
	//float brightness = smoothstep(0.5,0.2,dist);
	//FragColor = texColor * brightness;
	//FragColor = texture(texture2, screenUV);
	//float flicker = 1.0 - (sin(time * 20.0) * 0.1);
	FragColor = brightness * mix(texture(texture1, TexCoord),
					texture(texture2, vec2(TexCoord.x, TexCoord.y)),rate);
}