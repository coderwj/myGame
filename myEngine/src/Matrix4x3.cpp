#include "Matrix4x3.h"
#include "Vector3.h"
#include "RotationMatrix.h"

void Matrix4x3::identity()
{
	m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
	tx = 0.0f; ty = 0.0f; tz = 1.0f;
}

void Matrix4x3::zeroTranslation()
{
	tx = ty = tz = 0.0f;
}

void Matrix4x3::setTranslation(const Vector3 &v)
{
	tx = v.x; ty = v.y; tz = v.z;
}

void Matrix4x3::setupTranslation(const Vector3 &v)
{
	m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
	tx = v.x; ty = v.y; tz = v.z;
}

void Matrix4x3::setupLocalToParent(const Vector3 &pos, const EulerAngles &orient)
{
	RotationMatrix orientMatrix;
	orientMatrix.setup(orient);

	setupLocalToParent(pos, orientMatrix);
}

void Matrix4x3::setupLocalToParent(const Vector3 &pos, const RotationMatrix &orient)
{
	m11 = orient.m11; m12 = orient.m21; m13 = orient.m31;
	m21 = orient.m12; m22 = orient.m22; m23 = orient.m32;
	m31 = orient.m13; m32 = orient.m23; m33 = orient.m33;
	tx = pos.x; ty = pos.y; tz = pos.z;
}

void Matrix4x3::setupParentToLocal(const Vector3 &pos, const EulerAngles &orient)
{
	RotationMatrix orientMatrix;
	orientMatrix.setup(orient);

	setupParentToLocal(pos, orientMatrix);
}

void Matrix4x3::setupParentToLocal(const Vector3 &pos, const RotationMatrix &orient)
{
	m11 = orient.m11; m12 = orient.m12; m13 = orient.m13;
	m21 = orient.m21; m22 = orient.m22; m23 = orient.m23;
	m31 = orient.m31; m32 = orient.m32; m33 = orient.m33;
	tx = -(pos.x * m11 + pos.y * m21 + pos.z * m31);
	ty = -(pos.x * m12 + pos.y * m22 + pos.z * m32);
	tz = -(pos.x * m13 + pos.y * m23 + pos.z * m33);
}

void Matrix4x3::setupRotate(int axis, float theta)
{

}

void Matrix4x3::setupRotate(const Vector3 &axis, float theta)
{

}

void Matrix4x3::fromQuaternion(const Quaternion &q)
{

}

void Matrix4x3::setupScale(const Vector3 &s)
{
	m11 *= s.x; m12 *= s.x; m13 *= s.x;
	m21 *= s.y; m22 *= s.y; m23 *= s.y;
	m31 *= s.z; m32 *= s.z; m33 *= s.z;
}

void Matrix4x3::setupLookAt(const Vector3 &eye, const Vector3 &center, const Vector3 &up)
{
	Vector3 f = (center - eye).normalize();
	Vector3 u = (up).normalize();
	Vector3 s = (crossVector(f, u)).normalize();
	u = crossVector(s, f);

	m11 = s.x;
	m21 = s.y;
	m31 = s.z;

	m12 = u.x;
	m22 = u.y;
	m32 = u.z;

	m13 = -f.x;
	m23 = -f.y;
	m33 = -f.z;

	tx = -(s * eye);
	ty = -(u * eye);
	tz = (f * eye);
}

void Matrix4x3::setupPerspective(float fovy, float aspect, float zNear, float zFar)
{
	float range = tan(toRad(fovy * 0.5f)) * zNear;
	float left = -range * aspect;
	float right = range * aspect;
	float bottom = -range;
	float top = range;

	m11 = (2.0f * zNear) / (right - left);
	m22 = (2.0f * zNear) / (top - bottom);
	m33 = - (zFar + zNear) / (zFar - zNear);
	m34 = -1.0f;
	m43 = -(2.0f * zFar * zNear) / (zFar - zNear);
}
