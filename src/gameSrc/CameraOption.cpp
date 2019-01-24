#include "CameraOption.h"

#include "Camera.h"
#include "Character.h"

using namespace myEngine;

namespace myGame
{
	const float SPEED = 2.f;
	const float ROTATE_SPEED = 0.002f;
	const float SENSITIVTY = 0.1f;
	
	CameraOption::CameraOption()
	: m_moveSpeed(SPEED)
	, m_rotateSpeed(ROTATE_SPEED)
	, m_mouseSensitivity(SENSITIVTY)
	{
	
	}
	
	CameraOption::~CameraOption()
	{
	
	}
	
	void CameraOption::processKeyboard(CameraMoveDir direction, float deltaTime)
	{
		if (NULL == m_camera)
			return;
		float velocity = m_moveSpeed * deltaTime;
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
			float angle = m_rotateSpeed * deltaTime;
			if (direction == ROTATELEFT)
			{
				m_camera->rotateLeft(angle);
			}
			if (direction == ROTATERIGHT)
			{
				m_camera->rotateRight(angle);
			}
			if (direction == ROTATEUP)
			{
				m_camera->rotateUp(angle);
			}
			if (direction == ROTATEDOWN)
			{
				m_camera->rotateDown(angle);
			}
		}
	}
	
	void CameraOption::processMouseMovement(float xoffset, float yoffset)
	{

	}
	
	void CameraOption::processMouseScroll(float yoffset)
	{
		if (nullptr == m_camera)
			return;
		float velocity = m_moveSpeed * yoffset * 0.01f;
		m_camera->moveFront(velocity);
	}

	void CameraOption::resetCameraPos()
	{
		if (nullptr == m_camera)
			return;
		m_camera->setPosition(Vector3(0.f, 4.f, 6.f));
		m_camera->SetFocusPos(Vector3(0.f, 0.f, 0.f));
	}
}

