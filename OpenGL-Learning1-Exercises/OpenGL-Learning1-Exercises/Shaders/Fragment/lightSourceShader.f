#version 330 core

out vec4 FragColor;

uniform vec3 rbgVector;

void main() {
    FragColor = vec4(rbgVector, 1.0);
}