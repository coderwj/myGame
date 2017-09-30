#version 330
in vec2 ftexcoord;

layout(location = 0) out vec4 FragColor;
uniform sampler2D texture_diffuse1;
void main() {
	FragColor = texture(texture_diffuse1, ftexcoord);
}