#version 460 core

uniform vec3 u_pos;
uniform float u_pointSize;

void main() {
    gl_Position = vec4(u_pos, 1.0);
    gl_PointSize = u_pointSize;
}