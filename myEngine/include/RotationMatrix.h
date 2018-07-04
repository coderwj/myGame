#ifndef __ROTATIONMATRIX_H__
#define __ROTATIONMATRIX_H__

namespace myEngine
{
	class Vector3;
	class EulerAngles;
	class Quaternion;
	
	class RotationMatrix{
	public:
		float m11, m12, m13;
		float m21, m22, m23;
		float m31, m32, m33;
	
		void identity();
	
		void setup(const EulerAngles &orientation);
	
		void fromInertialToObjectQuaternion(const Quaternion &q);
	
		void fromObjectToInertialQuaternion(const Quaternion &q);
	
		Vector3 inertialToObject(const Vector3 &v) const;
		Vector3 objectToInertial(const Vector3 &v) const;
	};
}

#endif //__ROTATIONMATRIX_H__