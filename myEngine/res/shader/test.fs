#version 330
in vec3 fcolor;

layout(location = 0) out vec4 FragColor;
uniform sampler2D ourTexture;
uniform float time;

float my_random(float x, float y)
{
	return fract(cos(x * (12.9898) + y * (4.1414)) * 43758.5453);
}

#define r 250
float noise(float x, float y)
{
	int base_x = int(floor(x / r)); // 小组的x编号
	int base_y = int(floor(y / r)); // 小组的y编号
	x = fract(x / r); // 像素在小组内x方向的权值
	y = fract(y / r); // 像素在小组内y方向的权值
	float f_x = smoothstep(0, 1, x); // 平滑后的值
	float f_y = smoothstep(0, 1, y); // 平滑后的值
	return mix(
		mix(my_random(float(base_x), float(base_y)), my_random(float(base_x + 1), float(base_y)), f_x) // 第一次的x方向插值
			,mix(my_random(float(base_x), float(base_y + 1)), my_random(float(base_x + 1), float(base_y + 1)), f_x) // 第二次的x方向插值
			,f_y); // 最后的y方向插值
}

float fbm(float x, float y)
{
	float total = 0.0, amplitude = 1.0;
	for (int i = 0; i < 4; i++) // 4个函数叠加
	{
		total += noise(x, y) * amplitude; 
		x += x;  // 频率乘以2
		y += y;  // 频率乘以2
		amplitude *= 0.5; // 幅度乘以二分之一
	}
	return total;
} 

//Perlin噪声
void main() {
	float radf = my_random(gl_FragCoord.x, gl_FragCoord.y);
	float noif = fbm(gl_FragCoord.x, gl_FragCoord.y);
	//FragColor = vec4(abs(sin(time)), 0.0, 0.0, 1.0);
	//FragColor = vec4(fcolor.x, fcolor.y, fcolor.z, 1.0f);
	//FragColor = vec4(0.0, radf / 2.0, radf, 1.0);
	//FragColor = vec4(noif, noif, noif, 1.0);
	float z = fbm(gl_FragCoord.x - time * 100, gl_FragCoord.y - time * 100);
	float x = fbm(gl_FragCoord.x - time * 20, gl_FragCoord.y + time * 20);
	vec3 color1 = vec3(1.0, 0.618, 0.0);
	vec3 color2 = vec3(1.0, 0.0, 0.0);
	vec3 color3 = vec3(0.3, 0.0, 0.0);
	vec3 color4 = vec3(0.2, 0.2, 0.0);
	FragColor = vec4(mix(color2, color1, z) + mix(color3, color4, x), 1.0);
}
