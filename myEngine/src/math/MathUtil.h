#ifndef __MATHUTIL_H__
#define __MATHUTIL_H__

#include<cmath>

namespace myEngine
{
    const float kPi = 3.14159265f;
    const float k2Pi = kPi * 2.0f;
    const float kPiOver2 = kPi / 2.0f;
    const float k1OverPi = 1.0f / kPi;
    const float k1Over2Pi = 1.0f / k2Pi;
    
    //theta限制在 -pi 到 pi 之间
    extern float wrapPi(float theta);
    
    //safe反三角函数
    extern float safeAcos(float x);
    
    //角度转弧度
    inline float toRad(float f)
    {
        return f / 180.0f * kPi;
    }
    
    //弧度转角度
    inline float toTheta(float f)
    {
        return f / kPi * 180.0f;
    }
    
    //计算sin和cos
    inline void sinCos(float *resSin, float *resCos, float theta){
        //简化版
        *resSin = sin(theta);
        *resCos = cos(theta);
    }
    
    //浮点数比较
    inline bool fequal(float f1, float f2, float delta = 0.00001f)
    {
        return abs( f1 - f2) < delta;
    }
    
    inline bool fgreater(float f1, float f2, float delta = 0.00001f)
    {
        return f1 - f2 >= delta;
    }
    
    inline bool fsmaller(float f1, float f2, float delta = 0.00001f)
    {
        return f2 - f1 <= delta;
    }

    inline float lerpf(float from, float to, float k)
    {
        return from + k * (to - from);
    }
}

#endif //__MATHUTIL_H__