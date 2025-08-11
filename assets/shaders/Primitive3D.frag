#version 460 core

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_worldPos;

uniform sampler2D u_texture;
uniform int u_useTexture;         
uniform vec4 u_flatColor;          

uniform vec3 u_lightPos;           
uniform vec4 u_lightColor;         
uniform vec4 u_ambientColor;       
uniform vec3 u_viewPos;            

out vec4 FragColor;

void main() {
    // Choose between texture or flat color
    vec4 baseColor = u_useTexture == 1
        ? texture(u_texture, v_uv)
        : u_flatColor;

    // Lighting
    vec3 lightDir = normalize(u_lightPos - v_worldPos);
    vec3 norm = normalize(v_normal);
    float diff = max(dot(norm, normalize(lightDir)), 0.0);
    vec4 diffuse = diff * u_lightColor;

    vec4 ambient = u_ambientColor;

    // Simple specular
    vec3 viewDir = normalize(u_viewPos - v_worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec4 specular = spec * u_lightColor * 0.3; // 0.3 = specular strength

    vec4 lighting = (ambient + diffuse + specular);
    vec3 finalRGB = lighting.rgb * baseColor.rgb;

    // Multiply all alpha components together for correct transparency
    float finalAlpha = lighting.a * baseColor.a;

    FragColor = vec4(finalRGB, finalAlpha);
}