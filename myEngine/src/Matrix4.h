#pragma once

namespace myEngine
{
	class Vector3;
	class Quaternion;
	
	class Matrix4
	{
	public:
	
		union {
			struct
			{
				float m11, m12, m13, m14;
				float m21, m22, m23, m24;
				float m31, m32, m33, m34;
				float m41, m42, m43, m44;
			};
			float m[16];
		};
	
	    Matrix4() { identity(); }
		Matrix4(const float* data);
		Matrix4(const double* data);
	    Matrix4(const Matrix4& m);
		~Matrix4() {};
	
	    void identity();
	
	    void initWithScale(const Vector3& v);
	
	    void initWithRotate(const Vector3& axis, float theta);

		void initWithRotateQuaternion(const Quaternion& quat);
	
	    void initWithTranslate(const Vector3& v);

		void initWithScaleRotateTranslate(const Vector3& s, const Quaternion& r, const Vector3& t);
	
	    void initWithLookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
	
	    void initWithPerspective(float fovy, float aspect, float zN, float zF);

		void translate(const Vector3& v);

		void deCompose(Vector3& s, Quaternion& r, Vector3& t) const;
	
	    Matrix4& operator = (const Matrix4& m);
	
	    Matrix4 operator * (const Matrix4& m) const;
	    Matrix4& operator *= (const Matrix4& m);
	
	};
}