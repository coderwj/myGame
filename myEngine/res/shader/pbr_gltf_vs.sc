$input a_Position, a_Normal, a_Tangent, a_UV
$output v_Position, v_UV, v_TBNX, v_TBNY, v_TBNZ, v_Normal

uniform mat4 u_MVPMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_NormalMatrix;

void main()
{
  vec4 pos = mul(u_ModelMatrix, vec4(a_Position, 1.0));
  v_Position = pos.xyz / pos.w;

  #ifdef HAS_NORMALS
  #ifdef HAS_TANGENTS
  vec3 normalW = normalize(mul(u_NormalMatrix, vec4(a_Normal.xyz, 0.0)).xyz);
  vec3 tangentW = normalize(mul(u_ModelMatrix, vec4(a_Tangent.xyz, 0.0)).xyz);
  vec3 bitangentW = cross(normalW, tangentW) * a_Tangent.w;
  v_TBNX = tangentW;
  v_TBNY = bitangentW;
  v_TBNZ = normalW;
  #else // HAS_TANGENTS != 1
  v_Normal = normalize(mul(u_ModelMatrix, vec4(a_Normal.xyz, 0.0)).xyz);
  #endif
  #endif

  #ifdef HAS_UV
  v_UV = a_UV;
  #else
  v_UV = vec2(0.0, 0.0);
  #endif

  gl_Position = mul(u_MVPMatrix, vec4(a_Position, 1.0)); // needs w for proper perspective correction
}