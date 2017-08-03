#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 pNormal;

uniform sampler2D texture_diffuse1;
uniform vec3 light_color;
uniform vec3 light_dir;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
    vec3 lightdir = normalize(light_dir);
    vec3 nor = normalize(pNormal);
    float diff = max(dot(norm, lightDir), 0.0);
}