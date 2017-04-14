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

	void identity();

	void zeroTranslation();
	void setTranslation(const Vector3 &v);
	void setupTranslation(const Vector3 &v);

	void setupLocalToParent(const Vector3 &pos, const EulerAngles &orient);
	void setupLocalToParent(const Vector3 &pos, const RotationMatrix &orient);
	void setupParentToLocal(const Vector3 &pos, const EulerAngles &orient);
	void setupParentToLocal(const Vector3 &pos, const RotationMatrix &orient);

	void setupRotate(int axis, float theta);
};

#endif // __MATRIX4X3_H__