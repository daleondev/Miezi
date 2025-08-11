#version 460 core

uniform vec3 u_pos;

void main() {
    gl_Position = vec4(u_pos, 1.0);
}