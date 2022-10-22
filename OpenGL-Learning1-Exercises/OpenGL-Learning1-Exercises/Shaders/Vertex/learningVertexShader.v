#version 330 core
layout(location = 0) in vec3 aPos; // "position" has attribute position 0
layout(location = 1) in vec3 aColor; // "color" has attribute position 1
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec2 TexCoord;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor; // output color
    TexCoord = aTexCoord;
}