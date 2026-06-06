#version 330 core

out vec4 o_Color;

// 1. Interpolated data incoming from Vertex Shader
in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TexCoord;
in mat3 v_TBN;

// 2. Texture Samplers (Renderer3D binds Diffuse to slot 0, Specular to slot 1)
uniform sampler2D u_DiffuseMap;
uniform sampler2D u_SpecularMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_EmissionMap;

void main()
{
    // --- Light Setup (Hardcoded for testing, usually passed via uniforms) ---
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.6)); // Light coming from top-right-front
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    
    // 1. Obtain the normal from the normal map in range [0, 1]
    vec3 normalMapValue = texture(u_NormalMap, v_TexCoord).rgb;
    // Transform normal vector to range [-1, 1]
    normalMapValue = normalize(normalMapValue * 2.0 - 1.0);   
    // Apply TBN matrix to align the bump map with the physical 3D world
    vec3 norm = normalize(v_TBN * normalMapValue);

    // Extract base colors from textures
    vec4 diffuseTexColor = texture(u_DiffuseMap, v_TexCoord);

    if(diffuseTexColor.a < 0.1)
        discard;

    vec4 specularTexColor = texture(u_SpecularMap, v_TexCoord);
    vec3 emissionColor = texture(u_EmissionMap, v_TexCoord).rgb; // Glowing parts!

    // --- 1. Ambient Lighting ---
    // Ensures shadows aren't pitch black
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * diffuseTexColor.rgb;

    // --- 2. Diffuse Lighting ---
    // Calculates how directly the light hits the face
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * diffuseTexColor.rgb;

    // --- 3. Specular Lighting (Shiny highlights) ---
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-v_FragPos); // Assuming a simplified view from origin
    vec3 reflectDir = reflect(-lightDir, norm);
    
    // 32.0 is the 'shininess' factor. Higher = smaller, sharper highlights
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); 
    
    // Multiply by the specular map (black pixels = no shine, white pixels = max shine)
    vec3 specular = specularStrength * spec * lightColor * specularTexColor.rgb;

    // --- Final Output ---
    vec3 finalLightResult = ambient + diffuse + specular + emissionColor;
    o_Color = vec4(finalLightResult, diffuseTexColor.a);
}