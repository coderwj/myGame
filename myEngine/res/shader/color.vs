#version 330
layout(location = 0) in vec3 vposition;
layout(location = 1) in vec3 normalvec;
layout(location = 2) in vec3 colorvec;

out vec3 fcolor;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
void main() {
   fcolor = vec3(float(colorvec.x)/255.0f, float(colorvec.y)/255.0f, float(colorvec.z)/255.0f);
   gl_Position = Projection * View * Model * vec4(vposition, 1.0);
   //gl_Color = uvec4(fcolor, 255);
}