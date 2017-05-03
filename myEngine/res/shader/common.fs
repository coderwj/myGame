#version 330
in vec3 fcolor;
in vec2 ftexcoord;

layout(location = 0) out vec4 FragColor;
uniform sampler2D ourTexture;
void main() {
	//FragColor = vec4(fcolor, 1.0);
	FragColor = texture(ourTexture, ftexcoord);
}