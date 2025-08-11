#version 460 core

uniform vec4 u_color;

out vec4 FragColor;

void main() {
    float dist = length(gl_PointCoord - vec2(0.5));
    float alpha = smoothstep(0.5, 0.45, dist);
    FragColor = vec4(u_color.xyz, u_color.a*alpha);
}