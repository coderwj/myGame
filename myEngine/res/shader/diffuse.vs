
$input a_position, a_color, a_normal, a_tangent, a_texcoord0, a_indices, a_weight
$output v_Position, v_UV, v_TBNX, v_TBNY, v_TBNZ, v_Normal, v_Color

#include "common.sh"

#ifdef HAS_SKIN
uniform mat4 u_JointMatrixs[64];
#endif

uniform mat4 u_ModelMatrix;
uniform mat4 u_WorldMatrix;
uniform mat4 u_VPMatrix;

void main()
{

#ifdef HAS_SKIN
  mat4 skinMat = a_weight.x * u_JointMatrixs[int(a_indices.x)] +
                 a_weight.y * u_JointMatrixs[int(a_indices.y)] +
                 a_weight.z * u_JointMatrixs[int(a_indices.z)] +
                 a_weight.w * u_JointMatrixs[int(a_indices.w)];
  mat4 worldMatrix = u_WorldMatrix * u_ModelMatrix * skinMat;
#else // !HAS_SKIN
  mat4 worldMatrix = u_WorldMatrix * u_ModelMatrix;
#endif

  vec4 pos = mul(worldMatrix, vec4(a_position, 1.0));
  v_Position = pos.xyz / pos.w;

#ifdef HAS_UV
  v_UV = a_texcoord0;
#else
  v_UV = vec2(0.0, 0.0);
#endif

  gl_Position = mul(u_VPMatrix, pos);
  
}