#include <assert.h>
#include <cmath>

#include "Quaternion.h"
#include "MathUtil.h"
#include "Vector3.h"
#include "EulerAngles.h"
#include "Matrix4.h"

namespace myEngine
{
	const Quaternion Quaternion::IDENTITY(1.f, 0.f, 0.f, 0.f);

	Quaternion::Quaternion()
	{
		identity();
	}
	Quaternion::Quaternion(float _x, float _y, float _z, float _w)
	:x(_x)
	,y(_y)
	,z(_z)
	,w(_w)
	{
	}
	void Quaternion::setToRotateAboutX(float theta){
		float thetaOver2 = theta * 0.5f;
		w = cos(thetaOver2);
		x = sin(thetaOver2);
		y = 0.0f;
		z = 0.0f;
	}
	
	void Quaternion::setToRotateAboutY(float theta){
		float thetaOver2 = theta * 0.5f;
		w = cos(thetaOver2);
		x = 0.0f;
		y = sin(thetaOver2);
		z = 0.0f;
	}
	
	void Quaternion::setToRotateAboutZ(float theta){
		float thetaOver2 = theta * 0.5f;
		w = cos(thetaOver2);
		x = 0.0f;
		y = 0.0f;
		z = sin(thetaOver2);
	}
	
	void Quaternion::setToRotateAboutAxis(const Vector3 &axis, float theta){
		assert(fabs(vectorMod(axis) - 1.0f) < 0.01f);
	
		float thetaOver2 = theta * 0.5f;
		float sinThetaOver2 = sin(thetaOver2);
		w = cos(thetaOver2);
		x = axis.x * sinThetaOver2;
		y = axis.y * sinThetaOver2;
		z = axis.z * sinThetaOver2;
	}
	
	void Quaternion::setToRotateObjectToInertial(const EulerAngles &orientation){
		float sp, sb, sh;
		float cp, cb, ch;
	
		sinCos(&sp, &cp, orientation.pitch * 0.5f);
		sinCos(&sh, &ch, orientation.heading * 0.5f);
		sinCos(&sb, &cb, orientation.bank * 0.5f);
	
		w = ch * cp * cb + sh * sp * sb;
		x = ch * sp * cb + sh * cp * sb;
		y = -ch * sp * sb + sh * cp * cb;
		z = -sh * sp * cb + ch * cp * sb;
	}
	
	void Quaternion::setToRotateInertialToObject(const EulerAngles &orientation){
		float sp, sb, sh;
		float cp, cb, ch;
	
		sinCos(&sp, &cp, orientation.pitch * 0.5f);
		sinCos(&sh, &ch, orientation.heading * 0.5f);
		sinCos(&sb, &cb, orientation.bank * 0.5f);
	
		w = ch * cp * cb + sh * sp * sb;
		x = -ch * sp * cb - sh * cp * sb;
		y = ch * sp * sb - sh * cp * cb;
		z = sh * sp * cb - ch * cp * sb;
	}
	
	Quaternion Quaternion::operator *(const Quaternion &a) const{
		Quaternion res;
	
		res.w = w * a.w - x * a.x - y * a.y - z * a.z;
		res.x = w * a.x + x * a.w + y * a.z - z * a.y;
		res.y = w * a.y + y * a.w + z * a.x - x * a.z;
		res.z = w * a.z + z * a.w + x * a.y - y * a.x;
	
		return res;
	}
	
	Quaternion Quaternion::operator *=(const Quaternion &a){
		*this = *this * a;
	
		return *this;
	}
	
	void Quaternion::normalize(){
		float mag = (float)sqrt(w * w + x * x + y * y + z * z);
		if(mag > 0.0f)
		{
			float oneOverMag = 1.0f / mag;
			w *= oneOverMag;
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
		}
		else
		{
			assert(false);
			identity();
		}
	}
	
	float Quaternion::getRotationAngle() const{
		float thetaOver2 = safeAcos(w);
		return thetaOver2 * 2.0f;
	}
	
	Vector3 Quaternion::getRotationAxis() const{
		float sinThetaOver2Sq = 1.0f - w * w;
		if(sinThetaOver2Sq <= 0.0f)
		{
			return Vector3(1.0f, 0.0f, 0.0f);
		}
		float oneOverSinThetaOver2 = 1.0f / sqrt(sinThetaOver2Sq);
	
		return Vector3(
			x * oneOverSinThetaOver2,
			y * oneOverSinThetaOver2,
			z * oneOverSinThetaOver2
			);
	}

	void Quaternion::rotateVec3(Vector3& outVec, const Vector3& vec) const
	{
		// nVidia SDK implementation
		Vector3 uv, uuv;
		Vector3 qvec(x, y, z);
		uv = crossVector(qvec, vec);
		uuv = crossVector(qvec, uv);
		uv *= 2.f * w;
		uuv *= 2.f;

		outVec = vec + uv + uuv;
	}

	Vector3 Quaternion::operator* (const Vector3& v) const
	{
		// nVidia SDK implementation
		Vector3 uv, uuv;
		Vector3 qvec(x, y, z);
		uv = crossVector(qvec, v);
		uuv = crossVector(qvec, uv);
		uv *= 2.f * w;
		uuv *= 2.f;

		return v + uv + uuv;
	}

	void Quaternion::toMat4(Matrix4& mat) const
	{
		float xs = x * 2.0f;
		float ys = y * 2.0f;
		float zs = z * 2.0f;
		float wx = w * xs;
		float wy = w * ys;
		float wz = w * zs;
		float xx = x * xs;
		float xy = x * ys;
		float xz = x * zs;
		float yy = y * ys;
		float yz = y * zs;
		float zz = z * zs;

		mat.m11 = 1.0f - (yy + zz);	mat.m12 = xy + wz;			mat.m13 = xz - wy;			mat.m14 = 0.0f;
		mat.m21 = xy - wz;			mat.m22 = 1.0f - (xx + zz);	mat.m23 = yz + wx;			mat.m24 = 0.0f;
		mat.m31 = xz + wy;			mat.m32 = yz - wx;			mat.m33 = 1.0f - (xx + yy);	mat.m34 = 0.0f;
		mat.m41 = 0.0f;				mat.m42 = 0.0f;				mat.m43 = 0.0f;				mat.m44 = 1.0f;
	}

	float dotProduct(const Quaternion &a, const Quaternion &b) {
		return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
	}

	Quaternion slerp(const Quaternion &q0, const Quaternion &q1, float t) {
		if (t < 0.0f) return q0;
		if (t > 1.0f) return q1;
		float cosOmega = dotProduct(q0, q1);

		float q1w = q1.w;
		float q1x = q1.x;
		float q1y = q1.y;
		float q1z = q1.z;
		if (cosOmega < 0.0f)
		{
			q1w = -q1w;
			q1x = -q1x;
			q1y = -q1y;
			q1z = -q1z;
			cosOmega = -cosOmega;
		}
		assert(cosOmega < 1.1f);
		float k0, k1;
		if (cosOmega > 0.9999f)
		{
			k0 = 1.0f - t;
			k1 = t;
		}
		else
		{
			float sinOmega = sqrt(1.0f - cosOmega * cosOmega);
			float omega = atan2(sinOmega, cosOmega);
			float oneOverSinOmega = 1.0f / sinOmega;

			k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
			k1 = sin(t * omega);
		}
		Quaternion res;
		res.x = k0 * q0.x + k1 * q1x;
		res.y = k0 * q0.y + k1 * q1y;
		res.z = k0 * q0.z + k1 * q1z;
		res.w = k0 * q0.w + k1 * q1w;

		return res;

	}

	Quaternion conjugate(const Quaternion &q) {
		Quaternion res;
		res.w = q.w;
		res.x = -q.x;
		res.y = -q.y;
		res.z = -q.z;
		return res;
	}

	Quaternion pow(const Quaternion &q, float exponent) {
		if (fabs(q.w) > 0.9999f)
		{
			return q;
		}
		float alpha = acos(q.w);
		float newAlpha = alpha * exponent;
		Quaternion res;
		res.w = cos(newAlpha);

		float k = sin(newAlpha) / sin(alpha);

		res.x = k * q.x;
		res.y = k * q.y;
		res.z = k * q.z;

		return res;
	}

}