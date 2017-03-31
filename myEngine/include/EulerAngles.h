#ifndef __EULERANGLES_H__
#define __EULERANGLES_H__

class Quaternioin;
class Matrix4x3;
class RotationMatrix;

class EulerAngles{
public:
	float heading;
	float pitch;
	float bank;

	EulerAngles(){}
	EulerAngles(float h, float p, float b): heading(h), pitch(p), bank(b) {}
	void identity(){pitch = heading = bank = 0.0f;}
	void canonize();

	void fromObjectToInertialQuaternion(const Quaternioin &q);
	void fromInertialToObjectQuaternion(const Quaternioin &q);

	void fromObjectToWorldMatrix(const Matrix4x3 &m);
	void fromWorldToObjectMatrix(const Matrix4x3 &m);

	void fromRotationMatrix(const RotationMatrix &m);
};

extern const EulerAngles kEulerAnglesIdentity;

#endif //__EULERANGLES_H__