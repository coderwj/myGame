#pragma once

namespace myEngine
{
	class Camera;
}

namespace myGame
{
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
	
		void processKeyboard(CameraMoveDir direction, float deltaTime);
	
		void processMouseMovement(float xoffset, float yoffset);
	
		void processMouseScroll(float yoffset);
	
		float getMoveSpeed() const { return m_moveSpeed; }
		void setMoveSpeed(float val) { m_moveSpeed = val; }
	
		float getRotateSpeed() const { return m_rotateSpeed; }
		void setRotateSpeed(float val) { m_rotateSpeed = val; }
	
		float getMouseSensitivity() const { return m_mouseSensitivity; }
		void setMouseSensitivity(float val) { m_mouseSensitivity = val; }

		void resetCameraPos();
	
	private:
		myEngine::Camera* m_camera;
	
		float m_moveSpeed;
		float m_rotateSpeed;
		float m_mouseSensitivity;
	};
}