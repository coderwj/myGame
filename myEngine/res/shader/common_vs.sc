$input a_position, a_normal, a_texcoord0
$output v_texcoord0

#include "bgfx_shader.sh"
#include "shaderlib.sh"

void main() {
   v_texcoord0 = a_texcoord0;
   gl_Position = u_modelViewProj * vec4(a_position, 1.0);
}
