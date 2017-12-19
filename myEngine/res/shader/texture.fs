#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 pNormal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 light_color;
uniform vec3 light_dir;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 view_pos;

void main()
{

    FragColor = texture(texture_diffuse1, TexCoords);

    vec3 ambient = light_color * Ka * FragColor.xyz;

    vec3 lightdir = normalize(light_dir);
    vec3 norm = normalize(pNormal);
    float diff = max(dot(norm, lightdir), 0.0);
    vec3 diffuse = light_color * Kd * diff * FragColor.xyz;

    vec3 viewDir = normalize(view_pos - FragPos);
    vec3 reflectDir = reflect(-lightdir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float specularStrength = 0.6;
    vec3 specular = light_color * Ks * spec * FragColor.xyz;

    FragColor = vec4((ambient + diffuse + specular), 1.0);
}