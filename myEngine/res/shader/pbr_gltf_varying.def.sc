
vec3 a_Position		: POSITION;
vec4 a_Normal		: NORMAL;
vec4 a_Tangent		: TANGENT;
vec4 a_UV	        : TEXCOORD0;

vec3 v_Position	        : TEXCOORD0 = vec3(0.0, 0.0, 0.0);
vec2 v_UV			    : TEXCOORD1 = vec2(0.0, 0.0);
vec3 v_TBN			    : TEXCOORD2 = mat3(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
vec3 v_Normal           : TEXCOORD3 = vec3(0.0, 0.0, 0.0);