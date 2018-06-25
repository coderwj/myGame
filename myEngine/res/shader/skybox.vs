#version 330
layout(location = 0) in vec3 vposition;

out vec3 ftexcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
   ftexcoord = vposition;
   gl_Position = projection * view * model * vec4(vposition, 1.0);
}
