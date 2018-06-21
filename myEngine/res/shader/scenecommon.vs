#version 330
layout(location = 0) in vec3 vposition;
layout(location = 1) in vec3 normalvec;
layout(location = 2) in vec2 texcoord;

out vec2 ftexcoord;
out float fogIntensity;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraWorldPos;
uniform vec3 fogcolor; //(r, g, b)
uniform vec3 fogpara; //(start, end, intensity)

//from 0.0 to 1.0
float computFog(float dis) {
    float percent = max(dis - fogpara.x, 0.0) / max(fogpara.y - fogpara.x, 0.1);
    return clamp(clamp(percent, 0.0, 1.0) * fogpara.z, 0.0, 1.0);
}

void main() {
   ftexcoord = texcoord;
   gl_Position = projection * view * model * vec4(vposition, 1.0);
   vec3 worldPos = (model * vec4(vposition, 1.0)).xyz;
   float dis = length(worldPos - cameraWorldPos);
   fogIntensity = computFog(dis);
}
