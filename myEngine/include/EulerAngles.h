#ifndef __EULERANGLES_H__
#define __EULERANGLES_H__

namespace myEngine
{
	class Quaternion;
	class Matrix4;
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
	
		void fromObjectToInertialQuaternion(const Quaternion &q);
		void fromInertialToObjectQuaternion(const Quaternion &q);
	
		void fromObjectToWorldMatrix(const Matrix4 &m);
		void fromWorldToObjectMatrix(const Matrix4 &m);
	
		void fromRotationMatrix(const RotationMatrix &m);
	};
}

#endif //__EULERANGLES_H__