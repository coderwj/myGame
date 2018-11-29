$input v_Position, v_UV, v_TBNX, v_TBNY, v_TBNZ, v_Normal, v_Color

#include "common.sh"

#ifdef HAS_BASECOLORMAP
SAMPLER2D(u_BaseColorSampler, 1);
#endif

uniform vec4 u_LightDirection;
uniform vec4 u_LightColor;

uniform vec4 u_BaseColorFactor;
uniform vec4 u_Camera;

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

void main()
{

#ifdef HAS_BASECOLORMAP
    vec4 baseColor = SRGBtoLINEAR(texture2D(u_BaseColorSampler, v_UV)) * u_BaseColorFactor;
#else
    vec4 baseColor = u_BaseColorFactor;
#endif
    vec3 color = baseColor.rgb;
    gl_FragColor = vec4(pow(color, vec3_splat(1.0 / 2.2)), baseColor.a); // LineartoSRGB

}
