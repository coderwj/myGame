#version 330
in vec3 fcolor;

layout(location = 0) out vec4 FragColor;
uniform sampler2D ourTexture;
void main() {
	FragColor = vec4(fcolor.x, fcolor.y, fcolor.z, 1.0f);
}