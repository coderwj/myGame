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
    vec3 ambient = light_color * Ka;

    FragColor = texture(texture_diffuse1, TexCoords);
    vec3 lightdir = normalize(light_dir);
    vec3 nor = normalize(pNormal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light_color * Kd * diff;
}