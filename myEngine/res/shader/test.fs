#version 330
in vec3 fcolor;

layout(location = 0) out vec4 FragColor;
uniform sampler2D ourTexture;
uniform float time;
void main() {
	FragColor = vec4(abs(sin(time)),0.0,0.0,1.0);
	//FragColor = vec4(fcolor.x, fcolor.y, fcolor.z, 1.0f);
}