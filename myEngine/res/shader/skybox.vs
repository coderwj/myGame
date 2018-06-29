#version 330
layout(location = 0) in vec3 vposition;
layout(location = 1) in vec3 vtexcoord;

out vec3 ftexcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
   ftexcoord = vtexcoord;
   gl_Position = projection * view * model * vec4(vposition, 1.0);
}
