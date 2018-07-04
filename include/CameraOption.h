#pragma once

namespace myGame
{
	class Camera;
}

enum CameraMoveDir {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	ROTATELEFT,
	ROTATERIGHT,
	ROTATEUP,
	ROTATEDOWN
};

class CameraOption
{
public:
	CameraOption();
	~CameraOption();

	void ProcessKeyboard(CameraMoveDir direction, float deltaTime);

	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

	void ProcessMouseScroll(float yoffset);

	float getMovementSpeed() const { return m_movementSpeed; }
	void setMovementSpeed(float val) { m_movementSpeed = val; }

	float getRotateSpeed() const { return m_rotateSpeed; }
	void setRotateSpeed(float val) { m_rotateSpeed = val; }

	float getMouseSensitivity() const { return m_mouseSensitivity; }
	void setMouseSensitivity(float val) { m_mouseSensitivity = val; }

private:
	myEngine::Camera* m_camera;

	float m_movementSpeed;
	float m_rotateSpeed;
	float m_mouseSensitivity;
};