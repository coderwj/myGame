#version 330
in vec3 ftexcoord;

layout(location = 0) out vec4 FragColor;
uniform samplerCube skybox;
void main() {
	FragColor = texture(skybox, ftexcoord);
}
