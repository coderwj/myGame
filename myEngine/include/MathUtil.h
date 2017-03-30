#ifndef __MATHUTIL_H__
#define __MATHUTIL_H__

#include<math.h>

const float kPi = 3.14159265f;
const float k2Pi = kPi * 2.0f;
const float kPiOver2 = kPi / 2.0f;
const float k1OverPi = 1.0f / kPi;
const float k1Over2Pi = 1.0f / kPi;

//theta限制在 -pi 到 pi 之间
extern float wrapPi(float theta);

//safe反三角函数
extern float safeAcos(float x);

//计算sin和cos
inline void sinCos(float *resSin, float *resCos, float theta){
	//简化版
	*resSin = sin(theta);
	*resCos = cos(theta);
}

#endif //__MATHUTIL_H__