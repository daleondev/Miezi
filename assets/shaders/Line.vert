#version 460 core
layout(location = 0) in vec3 a_pos;

uniform mat4 u_viewProjection;
uniform mat4 u_model;

void main() {
    gl_Position = u_viewProjection * u_model * vec4(a_pos, 1.0);
}