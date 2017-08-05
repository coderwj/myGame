#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 pNormal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 light_color;
uniform vec3 light_dir;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    pNormal = aNormal;
    FragPos = gl_Position.xyz;
}