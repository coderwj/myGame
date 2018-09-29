$input a_position, a_normal, a_tangent, a_texcoord0, a_indices, a_weight
$output v_Position, v_UV, v_TBNX, v_TBNY, v_TBNZ, v_Normal

#include "common.sh"

#ifdef HAS_SKIN
uniform mat4 u_JointMatrixs[72];
#endif

uniform mat4 u_MVPMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_NormalMatrix;

void main()
{

#ifdef HAS_SKIN
  mat4 skinMat = a_weight.x * u_JointMatrixs[a_indices] +
                 a_weight.y * u_JointMatrixs[a_indices] +
                 a_weight.z * u_JointMatrixs[a_indices] +
                 a_weight.w * u_JointMatrixs[a_indices];
  u_ModelMatrix = u_ModelMatrix * skinMat;
  u_NormalMatrix = u_NormalMatrix * skinMat;
#endif

  vec4 pos = mul(u_ModelMatrix, vec4(a_position, 1.0));
  v_Position = pos.xyz / pos.w;

#ifdef HAS_NORMALS
#ifdef HAS_TANGENTS
  vec3 normalW = normalize(mul(u_NormalMatrix, vec4(a_normal.xyz, 0.0)).xyz);
  vec3 tangentW = normalize(mul(u_ModelMatrix, vec4(a_tangent.xyz, 0.0)).xyz);
  vec3 bitangentW = cross(normalW, tangentW) * a_tangent.w;
  v_TBNX = tangentW;
  v_TBNY = bitangentW;
  v_TBNZ = normalW;
#else // HAS_TANGENTS != 1
  v_Normal = normalize(mul(u_ModelMatrix, vec4(a_normal.xyz, 0.0)).xyz);
#endif
#endif

#ifdef HAS_UV
  v_UV = a_texcoord0;
#else
  v_UV = vec2(0.0, 0.0);
#endif

  gl_Position = mul(u_MVPMatrix, vec4(a_position, 1.0)); // needs w for proper perspective correction
  
}