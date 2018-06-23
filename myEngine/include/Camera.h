#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "MyEngineCore.h"
#include <vector>

enum CameraMove {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
	ROTATELEFT,
	ROTATERIGHT,
	ROTATEUP,
	ROTATEDOWN
};

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  2.f;
const GLfloat SENSITIVTY =  0.1f;

class Camera
{
public:
    // Camera Attributes
    Vector3 Position;
    Vector3 Front;
    Vector3 Up;
    Vector3 Right;
    Vector3 WorldUp;

    // Eular Angles
    GLfloat Yaw;
    GLfloat Pitch;

    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;

    Camera(Vector3 position = Vector3(0.0f, 0.0f, 0.0f),
           Vector3 up = Vector3(0.0f, 1.0f, 0.0f),
           GLfloat yaw = YAW,
           GLfloat pitch = PITCH);

    Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
           GLfloat upX, GLfloat upY, GLfloat upZ,
           GLfloat yaw, GLfloat pitch);

    Matrix4 GetViewMatrix() const;
	Matrix4 GetProjectMatrix() const;

    void ProcessKeyboard(CameraMove direction, GLfloat deltaTime);

    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

    void ProcessMouseScroll(GLfloat yoffset);

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

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors();

	float	m_nearClip;
	float	m_farClip;

	float	m_fov;

	float	m_aspect;			// viewProtWidth / viewProtHeight
	float	m_viewProtWidth;	// viewProtWidth
};
#endif