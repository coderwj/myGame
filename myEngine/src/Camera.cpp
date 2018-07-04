#include "Camera.h"

#include "MyEngineCore.h"
#include <vector>

//frustum
const float NEAR_CLIP = 0.1f;
const float FAR_CLIP = 1000.f;
const float FOV = 45.0f;
const float VIEW_PORT_WIDTH = 1280.f;
const float ASPECT = 1.777778f;

namespace myEngine
{
	Camera::Camera()
	: m_position(Vector3::UNIT_Z)
	, m_front(-Vector3::UNIT_Z)
	, m_rotate(0.f)
	, m_nearClip(NEAR_CLIP)
	, m_farClip(FAR_CLIP)
	, m_fov(FOV)
	, m_aspect(ASPECT)
	, m_viewProtWidth(VIEW_PORT_WIDTH)
	{

	}
	
	Camera::Camera(const Vector3& pos, const Vector3& focusPos)
	: m_position(pos)
	, m_front(focusPos - pos)
	, m_rotate(0.f)
	, m_nearClip(NEAR_CLIP)
	, m_farClip(FAR_CLIP)
	, m_fov(FOV)
	, m_aspect(ASPECT)
	, m_viewProtWidth(VIEW_PORT_WIDTH)
	{

	}
	
	Matrix4 Camera::GetViewMatrix() const
	{
	    Matrix4 result;
	    result.initWithLookAt(m_position, m_position + m_front, _getRight());
	    return result;
	}
	
	Matrix4 Camera::GetProjectMatrix() const
	{
		Matrix4 result;
		result.initWithPerspective(getFov(), getAspect(), getNearClip(), getFarClip());
		return result;
	}
	
	//void Camera::updateCameraVectors()
	//{
	//    // Calculate the new Front vector
	//    Vector3 front;
	//    front.x = cos(toRad(Pitch)) * cos(toRad(Yaw));
	//    front.y = sin(toRad(Pitch));
	//    front.z = cos(toRad(Pitch)) * sin(toRad(Yaw));
	//    Front = front.normalize();
	//    // Also re-calculate the Right and Up vector
	//    Right = crossVector(Front, WorldUp).normalize();
	//    Up    = crossVector(Right, Front).normalize();
	//}
	
	void Camera::SetFocusPos(Vector3 fpos)
	{
		Vector3 dir = (fpos - m_position).normalize();
		if (dir == Vector3(0.f))
			return;
		m_front = dir;
	
		//Pitch = toTheta(::asinf(dir.y));
		//if (Pitch > 89.0f)
		//	Pitch = 89.0f;
		//if (Pitch < -89.0f)
		//	Pitch = -89.0f;

		//if (fequal(dir.x, 0.f))
		//{
		//	if (fequal(dir.z, 0.f))
		//		Yaw = 0.f;
		//	else if (dir.z > 0.f)
		//		Yaw = 90.f;
		//	else
		//		Yaw = -90.f;
		//}
		//else if (dir.x > 0.f)
		//{
		//	Yaw = toTheta(::atanf(dir.z / dir.x));
		//}
		//else
		//{
		//	if (dir.z >= 0.f)
		//		Yaw = 180.f + toTheta(::atanf(dir.z / dir.x));
		//	else
		//		Yaw = -180.f + toTheta(::atanf(dir.z / dir.x));
		//}
	
	}

	void Camera::moveUp(float dis)
	{
		m_position += _getUp() * dis;
	}

	void Camera::moveRight(float dis)
	{
		m_position += _getRight() * dis;
	}

	Vector3 Camera::_getRight() const
	{
		Vector3 _right = crossVector(m_front, Vector3::UNIT_Z);
		Quaternion _rotate;
		_rotate.setToRotateAboutAxis(m_front, m_rotate);
		Vector3 _realRight = _right;
		_rotate.rotateVec3(_realRight, _right);
		return _realRight;
	}

	Vector3 Camera::_getUp() const
	{
		return crossVector(_getRight(), m_front);
	}

}