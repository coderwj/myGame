#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <cmath>

namespace myEngine
{
	class Vector3
	{
	public:
		float x, y, z;
	
		static const Vector3 ZERO;
		static const Vector3 UNIT_X;
		static const Vector3 UNIT_Y;
		static const Vector3 UNIT_Z;
	
	
		Vector3(){}
		Vector3(const Vector3 &v): x(v.x), y(v.y), z(v.z){}
		Vector3(float xx, float yy, float zz): x(xx), y(yy), z(zz){}
		Vector3(float vv): x(vv), y(vv), z(vv){}
	
		Vector3 & operator =(const Vector3 & v){
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}
	
		bool operator ==(const Vector3 &v) const{
			return x == v.x && y == v.y && z == v.z;
		}
	
		bool operator !=(const Vector3 &v) const{
			return x != v.x || y != v.y || z != v.z;
		}
	
		void zero() {x = y = z = 0.0f;}
	
		Vector3 operator -() const{return Vector3(-x, -y, -z);}
	
		Vector3 operator +(const Vector3 &v) const{
			return Vector3(x + v.x, y + v.y, z + v.z);
		}
	
		Vector3 operator -(const Vector3 &v) const{
			return Vector3(x - v.x, y - v.y, z - v.z);
		}
	
		//multi with float
		Vector3 operator *(float k) const{
			return Vector3(k * x, k * y, k * z);
		}
	
		Vector3 operator /(float k) const{
			return Vector3(x / k, y / k, z /k);
		}
	
		Vector3 & operator +=(const Vector3 &v){
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
	
		Vector3 & operator -=(const Vector3 &v){
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
	
		Vector3 & operator *=(float k){
			x *= k;
			y *= k;
			z *= k;
			return *this;
		}
		Vector3 & operator /=(float k){
			float oneOverK = 1.0f / k;
			x *= oneOverK;
			y *= oneOverK;
			z *= oneOverK;
			return *this;
		}
	
		Vector3 & normalize(){
			float sqMod = x*x + y*y + z*z;
			if(sqMod > 0.0f){
				float oneOverMod = 1.0f / sqrt(sqMod);
				x *= oneOverMod;
				y *= oneOverMod;
				z *= oneOverMod;
			}
			return *this;
		}
	
		float operator *(const Vector3 &v) const{
			return x * v.x + y * v.y + z * v.z;
		}

		Vector3& cross(const Vector3& v)
		{
			*this = Vector3(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x
			);
			return *this;
		}
	
	};
	
	inline float vectorMod(const Vector3 &v){
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}
	
	inline Vector3 crossVector(const Vector3 &v1, const Vector3 &v2){
		return Vector3(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
		);
	}
	
	inline Vector3 operator *(float k, const Vector3 &v){
		return Vector3(k * v.x, k * v.y, k * v.z);
	}
	
	inline float distance(const Vector3 &v1, const Vector3 &v2){
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;
		float dz = v1.z - v2.z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}
}


#endif //__VECTOR3_H__