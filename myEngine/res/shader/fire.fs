$input v_Position, v_UV, v_TBNX, v_TBNY, v_TBNZ, v_Normal, v_Color

#include "common.sh"

#ifdef HAS_BASECOLORMAP
SAMPLER2D(u_BaseColorSampler, 1);
#endif

uniform vec4 u_LightDirection;
uniform vec4 u_LightColor;

uniform vec4 u_BaseColorFactor;
uniform vec4 u_Camera;

uniform vec4 u_Time;

vec4 SRGBtoLINEAR(vec4 srgbIn)
{
#ifdef MANUAL_SRGB
#ifdef SRGB_FAST_APPROXIMATION
    vec3 linOut = pow(srgbIn.xyz,vec3_splat(2.2));
#else //SRGB_FAST_APPROXIMATION
    vec3 bLess = step(vec3_splat(0.04045),srgbIn.xyz);
    vec3 linOut = mix( srgbIn.xyz/vec3_splat(12.92), pow((srgbIn.xyz+vec3_splat(0.055))/vec3_splat(1.055),vec3_splat(2.4)), bLess );
#endif //SRGB_FAST_APPROXIMATION
    return vec4(linOut,srgbIn.w);;
#else //MANUAL_SRGB
    return srgbIn;
#endif //MANUAL_SRGB
}


float my_random(float x, float y)
{
    return fract(cos(x * (12.9898) + y * (4.1414)) * 43758.5453);
}

#define r 25
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
    // float radf = my_random(gl_FragCoord.x, gl_FragCoord.y);
    // float noif = fbm(gl_FragCoord.x, gl_FragCoord.y);
    //FragColor = vec4(abs(sin(u_Time.x)), 0.0, 0.0, 1.0);
    //FragColor = vec4(fcolor.x, fcolor.y, fcolor.z, 1.0f);
    //FragColor = vec4(0.0, radf / 2.0, radf, 1.0);
    //FragColor = vec4(noif, noif, noif, 1.0);
    float z = fbm(gl_FragCoord.x - u_Time.x * 100, gl_FragCoord.y - u_Time.x * 100);
    float x = fbm(gl_FragCoord.x - u_Time.x * 20, gl_FragCoord.y + u_Time.x * 20);
    vec3 color1 = vec3(1.0, 0.618, 0.0);
    vec3 color2 = vec3(1.0, 0.0, 0.0);
    vec3 color3 = vec3(0.3, 0.0, 0.0);
    vec3 color4 = vec3(0.2, 0.2, 0.0);
    vec3 color = mix(color2, color1, z) + mix(color3, color4, x);

#ifdef HAS_BASECOLORMAP
    vec4 baseColor = SRGBtoLINEAR(texture2D(u_BaseColorSampler, v_UV)) * u_BaseColorFactor;
#else
    vec4 baseColor = u_BaseColorFactor;
#endif
    gl_FragColor = vec4(mix(baseColor.rgb, color, 1.0), 1.0);
}
