
vec3 a_position		: POSITION;
vec3 a_normal		: NORMAL;
vec4 a_tangent		: TANGENT;
vec2 a_texcoord0	: TEXCOORD0;

vec3 v_Position	        : TEXCOORD0 = vec3(0.0, 0.0, 0.0);
vec2 v_UV			    : TEXCOORD1 = vec2(0.0, 0.0);
vec3 v_TBNX			    : TEXCOORD2 = vec3(0.0, 0.0, 0.0);
vec3 v_TBNY			    : TEXCOORD3 = vec3(0.0, 0.0, 0.0);
vec3 v_TBNZ			    : TEXCOORD4 = vec3(0.0, 0.0, 0.0);
vec3 v_Normal           : TEXCOORD5 = vec3(0.0, 0.0, 0.0);
