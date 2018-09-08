$input a_Position

#include "common.sh"

//uniform mat4 u_MVPMatrix;

void main()
{
    //gl_Position = mul(u_MVPMatrix, vec4(a_Position, 1.0));
    gl_Position = vec4(a_Position, 1.0);
}
