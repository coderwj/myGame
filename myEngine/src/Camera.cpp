#include "Camera.h"

#include "MyEngineCore.h"
#include <vector>

const float NEAR_CLIP = 0.1f;
const float FAR_CLIP = 1000.f;
const float FOV = 45.0f;
const float ASPECT = 1.777778f;
const float VIEW_PORT_WIDTH = 1280.f;

Camera::Camera(Vector3 position, Vector3 up, GLfloat yaw, GLfloat pitch):
Front(Vector3(0.0f, 0.0f, -1.0f))
, MovementSpeed(SPEED)
, RotateSpeed(ROTATE_SPEED)
, MouseSensitivity(SENSITIVTY)
, m_nearClip(NEAR_CLIP)
, m_farClip(FAR_CLIP)
, m_fov(FOV)
, m_aspect(ASPECT)
, m_viewProtWidth(VIEW_PORT_WIDTH)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch):
Front(Vector3(0.0f, 0.0f, -1.0f))
, MovementSpeed(SPEED)
, RotateSpeed(ROTATE_SPEED)
, MouseSensitivity(SENSITIVTY)
, m_nearClip(NEAR_CLIP)
, m_farClip(FAR_CLIP)
, m_fov(FOV)
, m_aspect(ASPECT)
, m_viewProtWidth(VIEW_PORT_WIDTH)
{
    Position = Vector3(posX, posY, posZ);
    WorldUp = Vector3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Matrix4 Camera::GetViewMatrix() const
{
    Matrix4 result;
    result.initWithLookAt(Position, Position + Front, Up);
    return result;
}

Matrix4 Camera::GetProjectMatrix() const
{
	Matrix4 result;
	result.initWithPerspective(getFov(), getAspect(), getNearClip(), getFarClip());
	return result;
}

void Camera::ProcessKeyboard(CameraMove direction, GLfloat deltaTime)
{
    GLfloat velocity = MovementSpeed * deltaTime;
	GLfloat angle = RotateSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Up * velocity;
    if (direction == BACKWARD)
        Position -= Up * velocity;
    if (direction == LEFT)
        Position -= Right * velocity * 2.f;
    if (direction == RIGHT)
        Position += Right * velocity * 2.f;
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

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
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

void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    Vector3 front;
    front.x = cos(toRad(Pitch)) * cos(toRad(Yaw));
    front.y = sin(toRad(Pitch));
    front.z = cos(toRad(Pitch)) * sin(toRad(Yaw));
    Front = front.normalize();
    // Also re-calculate the Right and Up vector
    Right = crossVector(Front, WorldUp).normalize();
    Up    = crossVector(Right, Front).normalize();
}

void Camera::SetFocusPos(Vector3 fpos)
{
	Vector3 dir = (fpos - Position).normalize();
	if (dir == Vector3(0.f))
		return;
	Front = dir;

	Pitch = toTheta(::asinf(dir.y));
	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	if (fequal(dir.x, 0.f))
	{
		if (fequal(dir.z, 0.f))
			Yaw = 0.f;
		else if (dir.z > 0.f)
			Yaw = 90.f;
		else
			Yaw = -90.f;
	}
	else if (dir.x > 0.f)
	{
		Yaw = toTheta(::atanf(dir.z / dir.x));
	}
	else
	{
		if (dir.z >= 0.f)
			Yaw = 180.f + toTheta(::atanf(dir.z / dir.x));
		else
			Yaw = -180.f + toTheta(::atanf(dir.z / dir.x));
	}

	// re-calculate the Right and Up vector
	Right = crossVector(Front, WorldUp).normalize();
	Up = crossVector(Right, Front).normalize();
}