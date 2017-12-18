#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include <cstring>

class Vector3;
class Quaternion;

#endif // __MATRIX_H__

class Matrix4
{
public:

    float m11, m12, m13, m14;
    float m21, m22, m23, m24;
    float m31, m32, m33, m34;
    float m41, m42, m43, m44;

    Matrix4() { identity(); };
    Matrix4(const Matrix4& m){memcpy(this, &m, sizeof(float) * 16);};
	~Matrix4() {};

    void identity();

    void initWithScale(const Vector3 &v);

    void initWithRotate(const Vector3 &axis, float theta);

    void initWithTranslate(const Vector3 &v);

    void initWithLookAt(const Vector3 &eye, const Vector3 &center, const Vector3 &up);

    void initWithPerspective(float fovy, float aspect, float zN, float zF);

    Matrix4& operator = (const Matrix4 &m);

    Matrix4 operator * (const Matrix4 &m) const;
    Matrix4& operator *= (const Matrix4 &m);

};