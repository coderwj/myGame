#version 330
layout(location = 0) in vec3 vposition;
layout(location = 1) in vec3 normalvec;
layout(location = 2) in vec3 colorvec;
layout(location = 3) in vec2 texcoord;
out vec4 fcolor;
out vec2 ftexcoord;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
void main() {
   fcolor = vec4(colorvec, 1.0);
   ftexcoord = texcoord;
   gl_Position = Projection * View * Model * vec4(vposition, 1.0);
}