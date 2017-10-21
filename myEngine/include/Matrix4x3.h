#ifndef __MATRIX4X3_H__
#define __MATRIX4X3_H__

class Vector3;
class EulerAngles;
class RotationMatrix;
class Quaternion;

class Matrix4x3
{
public:
	float m11, m12, m13;
	float m21, m22, m23;
	float m31, m32, m33;
	float tx, ty, tz;

	Matrix4x3()
	{
		identity();
	}

	void identity();

	void zeroTranslation();
	void setTranslation(const Vector3 &v);
	void setupTranslation(const Vector3 &v);

	void setupLocalToParent(const Vector3 &pos, const EulerAngles &orient);
	void setupLocalToParent(const Vector3 &pos, const RotationMatrix &orient);
	void setupParentToLocal(const Vector3 &pos, const EulerAngles &orient);
	void setupParentToLocal(const Vector3 &pos, const RotationMatrix &orient);

	void setupRotate(int axis, float theta);

	void setupRotate(const Vector3 &axis, float theta);

	void fromQuaternion(const Quaternion &q);

	void setupScale(const Vector3 &s);

	void setupLookAt(const Vector3 &eye, const Vector3 &center, const Vector3 &up);

	void setupPerspective(float fovy, float aspect, float zNear, float zFar);

};

#endif // __MATRIX4X3_H__