#include <cmath>

#include "MathUtil.h"
#include "Vector3.h"

namespace myEngine
{   
    float wrapPi(float theta){
        theta += kPi;
        theta -= floor(theta * k1Over2Pi) * k2Pi;
        theta -= kPi;
        return theta;
    }
    
    float safeAcos(float x){
        if(x <= -1.0f){
            return kPi;
        }
        if(x >= 1.0f){
            return 0;
        }
        return acos(x);
    }
}
