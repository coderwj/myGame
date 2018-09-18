$input a_position

#include "common.sh"

//uniform mat4 u_MVPMatrix;

void main()
{
    //gl_Position = mul(u_MVPMatrix, vec4(a_position, 1.0));
    gl_Position = vec4(a_position, 1.0);
}
