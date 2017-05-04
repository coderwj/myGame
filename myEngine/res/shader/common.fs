#version 330
in vec3 fcolor;
in vec2 ftexcoord;

layout(location = 0) out vec4 FragColor;
uniform sampler2D ourTexture;
void main() {
	//FragColor = vec4(fcolor, 1.0);
	//vec2 ftexcoord1 = vec2(100, 100);
	FragColor = texture(ourTexture, ftexcoord);
}