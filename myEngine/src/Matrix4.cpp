#include "Matrix4.h"
#include "Vector3.h"
#include "MathUtil.h"
#include "Quaternion.h"

#include<cstring>

namespace myEngine
{
	Matrix4::Matrix4(const float * data)
	{
		memcpy(this, data, sizeof(float) * 16);
	}
	Matrix4::Matrix4(const double * data)
	{
		memcpy(this, data, sizeof(float) * 16);
	}
	Matrix4::Matrix4(const Matrix4& m)
	{
		memcpy(this, &m, sizeof(float) * 16);
	}

	void Matrix4::identity()
	{
	    m11 = 1.0f; m12 = 0.0f; m13 = 0.0f; m14 = 0.0f;
	    m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; m24 = 0.0f;
	    m31 = 0.0f; m32 = 0.0f; m33 = 1.0f; m34 = 0.0f;
	    m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
	}
	
	
	void Matrix4::initWithScale(const Vector3 &v)
	{
	    m11 = v.x;  m12 = 0.0f; m13 = 0.0f; m14 = 0.0f;
	    m21 = 0.0f; m22 = v.y;  m23 = 0.0f; m24 = 0.0f;
	    m31 = 0.0f; m32 = 0.0f; m33 = v.z;  m34 = 0.0f;
	    m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
	}
	
	void Matrix4::initWithRotate(const Vector3 &axis, float theta)
	{
	    float x = axis.x;
	    float y = axis.y;
	    float z = axis.z;
	
	    float sinr, cosr;
	    sinCos(&sinr, &cosr, toRad(theta));
	
	    m11 = (x * x) * (1 - cosr) + cosr;
	    m12 = (x * y) * (1 - cosr) + (z * sinr);
	    m13 = (x * z) * (1 - cosr) - (y * sinr);
	    m14 = 0.0f;
	
	    m21 = (y * x) * (1 - cosr) - (z * sinr);
	    m22 = (y * y) * (1 - cosr) + cosr;
	    m23 = (y * z) * (1 - cosr) + (x * sinr);
	    m24 = 0.0f;
	
	    m31 = (z * x) * (1 - cosr) + (y * sinr);
	    m32 = (z * y) * (1 - cosr) - (x * sinr);
	    m33 = (z * z) * (1 - cosr) + cosr;
	    m34 = 0.0f;
	
	    m41 = 0.0f;
	    m42 = 0.0f;
	    m43 = 0.0f;
	    m44 = 1.0f;
	}

	void Matrix4::initWithRotateQuaternion(const Quaternion & quat)
	{
		// temp method. TODO..
		const Vector3& _axis = quat.getRotationAxis();
		float _theta = toTheta(quat.getRotationAngle());
		initWithRotate(_axis, _theta);
	}
	
	void Matrix4::initWithTranslate(const Vector3 &v)
	{
	    m11 = 1.0f; m12 = 0.0f; m13 = 0.0f; m14 = 0.0f;
	    m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; m24 = 0.0f;
	    m31 = 0.0f; m32 = 0.0f; m33 = 1.0f; m34 = 0.0f;
	    m41 = v.x;  m42 = v.y;  m43 = v.z;  m44 = 1.0f;
	}
	
	void Matrix4::initWithLookAt(const Vector3 &eye, const Vector3 &center, const Vector3 &right)
	{
	    Vector3 f = (center - eye).normalize();
	    Vector3 r = Vector3(right).normalize();
	    Vector3 u = (crossVector(r, f)).normalize();
	
	    m11 = r.x;				m12 = u.x;				m13 = -f.x;			m14 = 0.0f;
	    m21 = r.y;				m22 = u.y;				m23 = -f.y;			m24 = 0.0f;
	    m31 = r.z;				m32 = u.z;				m33 = -f.z;			m34 = 0.0f;
	    m41 = -(r.dot(eye));	m42 = -(u.dot(eye));	m43 = (f.dot(eye)); m44 = 1.0f;
	}
	
	void Matrix4::initWithPerspective(float fovy, float aspect, float zN, float zF)
	{
	
	    float range = tan(toRad(fovy * 0.5f)) * zN;
	    float left = -range * aspect;
	    float right = range * aspect;
	    float bottom = -range;
	    float top = range;
	
	    identity();
	    m11 = (2.0f * zN) / (right - left);
	    m22 = (2.0f * zN) / (top - bottom);
	    m33 = - (zF + zN) / (zF - zN);
	    m34 = -1.0f;
	    m43 = -(2.0f * zF * zN) / (zF - zN);
	    m44 = 0.0f;
	}

	void Matrix4::translate(const Vector3 & v)
	{
		m41 = v.x;  m42 = v.y;  m43 = v.z;
	}
	
	Matrix4& Matrix4::operator = (const Matrix4 &m)
	{
	    memcpy(this, &m, sizeof(float) * 16);
	    return *this;
	}
	
	Matrix4 Matrix4::operator * (const Matrix4 &m) const
	{
	    Matrix4 result;
	    result.m11 = m11 * m.m11 + m12 * m.m21 + m13 * m.m31 + m14 * m.m41;
	    result.m12 = m11 * m.m12 + m12 * m.m22 + m13 * m.m32 + m14 * m.m42;
	    result.m13 = m11 * m.m13 + m12 * m.m23 + m13 * m.m33 + m14 * m.m43;
	    result.m14 = m11 * m.m14 + m12 * m.m24 + m13 * m.m34 + m14 * m.m44;
	
	    result.m21 = m21 * m.m11 + m22 * m.m21 + m23 * m.m31 + m24 * m.m41;
	    result.m22 = m21 * m.m12 + m22 * m.m22 + m23 * m.m32 + m24 * m.m42;
	    result.m23 = m21 * m.m13 + m22 * m.m23 + m23 * m.m33 + m24 * m.m43;
	    result.m24 = m21 * m.m14 + m22 * m.m24 + m23 * m.m34 + m24 * m.m44;
	
	    result.m31 = m31 * m.m11 + m32 * m.m21 + m33 * m.m31 + m34 * m.m41;
	    result.m32 = m31 * m.m12 + m32 * m.m22 + m33 * m.m32 + m34 * m.m42;
	    result.m33 = m31 * m.m13 + m32 * m.m23 + m33 * m.m33 + m34 * m.m43;
	    result.m34 = m31 * m.m14 + m32 * m.m24 + m33 * m.m34 + m34 * m.m44;
	
	    result.m41 = m41 * m.m11 + m42 * m.m21 + m43 * m.m31 + m44 * m.m41;
	    result.m42 = m41 * m.m12 + m42 * m.m22 + m43 * m.m32 + m44 * m.m42;
	    result.m43 = m41 * m.m13 + m42 * m.m23 + m43 * m.m33 + m44 * m.m43;
	    result.m44 = m41 * m.m14 + m42 * m.m24 + m43 * m.m34 + m44 * m.m44;
	
	    return result;
	}
	
	Matrix4& Matrix4::operator *=(const Matrix4 &m)
	{
	    Matrix4 result;
	    result.m11 = m11 * m.m11 + m12 * m.m21 + m13 * m.m31 + m14 * m.m41;
	    result.m12 = m11 * m.m12 + m12 * m.m22 + m13 * m.m32 + m14 * m.m42;
	    result.m13 = m11 * m.m13 + m12 * m.m23 + m13 * m.m33 + m14 * m.m43;
	    result.m14 = m11 * m.m14 + m12 * m.m24 + m13 * m.m34 + m14 * m.m44;
	
	    result.m21 = m21 * m.m11 + m22 * m.m21 + m23 * m.m31 + m24 * m.m41;
	    result.m22 = m21 * m.m12 + m22 * m.m22 + m23 * m.m32 + m24 * m.m42;
	    result.m23 = m21 * m.m13 + m22 * m.m23 + m23 * m.m33 + m24 * m.m43;
	    result.m24 = m21 * m.m14 + m22 * m.m24 + m23 * m.m34 + m24 * m.m44;
	
	    result.m31 = m31 * m.m11 + m32 * m.m21 + m33 * m.m31 + m34 * m.m41;
	    result.m32 = m31 * m.m12 + m32 * m.m22 + m33 * m.m32 + m34 * m.m42;
	    result.m33 = m31 * m.m13 + m32 * m.m23 + m33 * m.m33 + m34 * m.m43;
	    result.m34 = m31 * m.m14 + m32 * m.m24 + m33 * m.m34 + m34 * m.m44;
	
	    result.m41 = m41 * m.m11 + m42 * m.m21 + m43 * m.m31 + m44 * m.m41;
	    result.m42 = m41 * m.m12 + m42 * m.m22 + m43 * m.m32 + m44 * m.m42;
	    result.m43 = m41 * m.m13 + m42 * m.m23 + m43 * m.m33 + m44 * m.m43;
	    result.m44 = m41 * m.m14 + m42 * m.m24 + m43 * m.m34 + m44 * m.m44;
	
	    *this = result;
	    return *this;
	}

}