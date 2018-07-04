#include "CameraOption.h"
#include "Camera.h"

using namespace myGame;

const float SPEED = 2.f;
const float ROTATE_SPEED = 45.f;
const float SENSITIVTY = 0.1f;

CameraOption::CameraOption()
: m_movementSpeed(SPEED)
, m_rotateSpeed(ROTATE_SPEED)
, m_mouseSensitivity(SENSITIVTY)
{

}

CameraOption::~CameraOption()
{

}

void CameraOption::ProcessKeyboard(CameraMoveDir direction, float deltaTime)
{
	if (NULL == m_camera)
		return;
	float velocity = getMovementSpeed() * deltaTime;
	if (direction == FORWARD)
		m_camera->moveUp(velocity);
	else if (direction == BACKWARD)
		m_camera->moveDown(velocity);
	else if (direction == LEFT)
		m_camera->moveLeft(velocity * 2.f);
	else if (direction == RIGHT)
		m_camera->moveRight(velocity * 2.f);
	else
	{
		float angle = getRotateSpeed() * deltaTime;
		if (direction == ROTATELEFT)
		{
			Yaw -= angle;
			updateCameraVectors();
		}
		if (direction == ROTATERIGHT)
		{
			Yaw += angle;
			updateCameraVectors();
		}
		if (direction == ROTATEUP)
		{
			Pitch += angle;
			if (Pitch > 89.f)
				Pitch = 89.f;
			updateCameraVectors();
		}
		if (direction == ROTATEDOWN)
		{
			Pitch -= angle;
			if (Pitch < -89.f)
				Pitch = -89.f;
			updateCameraVectors();
		}
	}
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
	GLfloat velocity = MovementSpeed * yoffset;
	Position += Front * velocity * 0.3f;
}

