#version 330
in vec2 ftexcoord;
in float fogIntensity;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform vec3 fogcolor; //(r, g, b)

void main() {
	FragColor = texture(texture_diffuse1, ftexcoord);
	FragColor = vec4(mix(FragColor.rgb, fogcolor, fogIntensity), FragColor.a);
}
