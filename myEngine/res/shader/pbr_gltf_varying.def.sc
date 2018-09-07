
vec3 a_Position		: POSITION;
vec3 a_Normal		: NORMAL;
vec4 a_Tangent		: TANGENT;
vec2 a_UV	        : TEXCOORD0;

vec3 v_Position	        : TEXCOORD0 = vec3(0.0, 0.0, 0.0);
vec2 v_UV			    : TEXCOORD1 = vec2(0.0, 0.0);
vec3 v_TBNX			    : TEXCOORD2 = vec3(0.0, 0.0, 0.0);
vec3 v_TBNY			    : TEXCOORD3 = vec3(0.0, 0.0, 0.0);
vec3 v_TBNZ			    : TEXCOORD4 = vec3(0.0, 0.0, 0.0);
vec3 v_Normal           : TEXCOORD5 = vec3(0.0, 0.0, 0.0);
