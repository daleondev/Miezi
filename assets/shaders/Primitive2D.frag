#version 460 core

in vec2 v_uv;

uniform sampler2D u_texture;
uniform int u_useTexture;         
uniform vec4 u_color;

out vec4 FragColor;

void main() {
    // Choose between texture or flat color
    FragColor = u_useTexture == 1
        ? texture(u_texture, v_uv)
        : u_color;
}