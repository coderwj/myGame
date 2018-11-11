#pragma once

#include <vector>

#include "Vector3.h"
#include "Matrix4.h"

namespace myEngine
{
	class Camera
	{
	public:

		Camera();
		Camera(const Vector3& pos, const Vector3& focusPos);
	
	    Matrix4 GetViewMatrix() const;
		Matrix4 GetProjectMatrix() const;

		const Vector3& getPosition() const { return m_position; }
		void setPosition(const Vector3& val) { m_position = val; }

		const Vector3& getFront() const { return m_front; }
		void setFront(Vector3 val) { m_front = val; }

		void SetFocusPos(Vector3 pos);
	
		void setNearClip(float c) { m_nearClip = c; }
		float getNearClip() const { return m_nearClip; }
	
		void setFarClip(float c) { m_farClip = c; }
		float getFarClip() const { return m_farClip; }
	
		void setFov(float f) { m_fov = f; }
		float getFov() const { return m_fov; }
	
		void setAspect(float a) { m_aspect = a; }
		float getAspect() const { return m_aspect; }
	
		void setViewPortWidth(float w) { m_viewProtWidth = w; }
		float getViewPortWidth() const { return m_viewProtWidth; }


		//move
		void moveUp(float dis);
		void moveDown(float dis) { moveUp(-dis); }
		void moveRight(float dis);
		void moveLeft(float dis) { moveRight(-dis); }
		void moveFront(float dis);
		void moveBack(float dis) { moveFront(-dis); }

		//rotate
		void rotateRight(float theta);
		void rotateLeft(float theta) { rotateRight(-theta); }
		void rotateUp(float theta);
		void rotateDown(float theta) { rotateUp(-theta); }

	private:
		Vector3 _getRight() const;
		Vector3 _getUp() const;

	private:

		Vector3 m_position;
		Vector3 m_front;

		float   m_rotate; // rotate, default 0, rad, from -Pi to Pi.
	
		float	m_nearClip;
		float	m_farClip;
		float	m_fov;
		float	m_aspect;			// viewProtWidth / viewProtHeight
		float	m_viewProtWidth;	// viewProtWidth
	};
}
