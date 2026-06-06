#version 330 core

// In 330, layout(location = 0) out vec4 is not supported. 
// Simply declaring an out vec4 binds it to the default render target.
out vec4 o_Color;

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_WorldPosition;

uniform vec4 u_Color;

void main()
{
    // 1. Ambient Lighting (Base fallback light so shadows aren't pitch black)
    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * u_Color.rgb;
    
    // 2. Diffuse Directional Lighting
    vec3 norm = normalize(v_Normal);
    // Arbitrary light source angle coming from front-top-right
    vec3 lightDir = normalize(vec3(0.4, 1.0, 0.6)); 
    
    // Calculate alignment with the light angle
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_Color.rgb;
    
    // 3. Combine results
    vec3 finalLightColor = ambient + diffuse;
    
    //o_Color = vec4(finalLightColor, u_Color.a);
    o_Color = vec4(u_Color);
}