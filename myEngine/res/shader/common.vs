#version 330
layout(location = 0) in vec4 vposition;
out vec4 fcolor;
void main() {
   fcolor = vec4(0.8,0.0,0.6,1.0);
   gl_Position = vposition;
}