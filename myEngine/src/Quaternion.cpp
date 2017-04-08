#include <assert.h>
#include <math.h>

#include "Quaternion.h"
#include "MathUtil.h"
#include "Vector3.h"
#include "EulerAngles.h"

const Quaternion kQuaternionIdentity = {1.0f, 0.0f, 0.0f, 0.0f};

void Quaternion::SetToRotateAboutX(float theta){
	float thetaOver2 = theta * 0.5f;
	w = cos(thetaOver2);
	x = sin(thetaOver2);
	y = 0.0f;
	z = 0.0f;
}

void Quaternion::SetToRotateAboutY(float theta){
	float thetaOver2 = theta * 0.5f;
	w = cos(thetaOver2);
	x = 0.0f;
	y = sin(thetaOver2);
	z = 0.0f;
}

void Quaternion::SetToRotateAboutZ(float theta){
	float thetaOver2 = theta * 0.5f;
	w = cos(thetaOver2);
	x = 0.0f;
	y = 0.0f;
	z = sin(thetaOver2);
}

void Quaternion::SetToRotateAboutAxis(const Vector3 &axis, float theta){
	assert(fabs(vectorMod(axis) - 1.0f) < 0.01f);

	float thetaOver2 = theta * 0.5f;
	float sinThetaOver2 = sin(thetaOver2);
	w = cos(thetaOver2);
	x = axis.x * sinThetaOver2;
	y = axis.y * sinThetaOver2;
	z = axis.z * sinThetaOver2;
}

void Quaternion::SetToRotateObjectToInertial(const EulerAngles &orientation){
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

void Quaternion::SetToRotateInertialToObject(const EulerAngles &orientation){
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
	res.x = w * a.x + x * a.w + z * a.y - y * a.z;
	res.y = w * a.y + y * a.w + x * a.z - z * a.x;
	res.z = w * a.z + z * a.w + y * a.x - x * a.y;

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

