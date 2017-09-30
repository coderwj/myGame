#version 330
layout(location = 0) in vec3 vposition;
layout(location = 1) in vec3 normalvec;
layout(location = 2) in vec2 texcoord;

out vec2 ftexcoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
   ftexcoord = texcoord;
   gl_Position = projection * view * model * vec4(vposition, 1.0);
}