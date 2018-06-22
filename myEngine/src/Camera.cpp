#include "Camera.h"

#include "MyEngineCore.h"
#include <vector>


Camera::Camera(Vector3 position, Vector3 up, GLfloat yaw, GLfloat pitch):
Front(Vector3(0.0f, 0.0f, -1.0f)),
MovementSpeed(SPEED),
MouseSensitivity(SENSITIVTY),
Zoom(ZOOM)
{
    this->Position = position;
    this->WorldUp = up;
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch):
Front(Vector3(0.0f, 0.0f, -1.0f)),
MovementSpeed(SPEED),
MouseSensitivity(SENSITIVTY),
Zoom(ZOOM)
{
    this->Position = Vector3(posX, posY, posZ);
    this->WorldUp = Vector3(upX, upY, upZ);
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

Matrix4 Camera::GetViewMatrix()
{
    Matrix4 result;
    result.initWithLookAt(this->Position, this->Position + this->Front, this->Up);
    return result;
}

void Camera::ProcessKeyboard(CameraMove direction, GLfloat deltaTime)
{
    GLfloat velocity = this->MovementSpeed * deltaTime;
    if (direction == FORWARD)
        this->Position += this->Up * velocity;
    if (direction == BACKWARD)
        this->Position -= this->Up * velocity;
    if (direction == LEFT)
        this->Position -= this->Right * velocity * 2.f;
    if (direction == RIGHT)
        this->Position += this->Right * velocity * 2.f;
	if (direction == ROTATELEFT)
	{
		this->Yaw -= 0.08f;
		this->updateCameraVectors();
	}
	if (direction == ROTATERIGHT)
	{
		this->Yaw += 0.08f;
		this->updateCameraVectors();
	}
	if (direction == ROTATEUP)
	{
		this->Pitch += 0.03f;
		if (this->Pitch > 89.f)
			this->Pitch = 89.f;
		this->updateCameraVectors();
	}
	if (direction == ROTATEDOWN)
	{
		this->Pitch -= 0.03f;
		if (this->Pitch < -89.f)
			this->Pitch = -89.f;
		this->updateCameraVectors();
	}
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw   += xoffset;
    this->Pitch += yoffset;

    if (constrainPitch)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }

    this->updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
	GLfloat velocity = this->MovementSpeed * yoffset;
	this->Position += this->Front * velocity * 0.2f;
}

void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    Vector3 front;
    front.x = cos(toRad(this->Pitch)) * cos(toRad(this->Yaw));
    front.y = sin(toRad(this->Pitch));
    front.z = cos(toRad(this->Pitch)) * sin(toRad(this->Yaw));
    this->Front = front.normalize();
    // Also re-calculate the Right and Up vector
    this->Right = crossVector(this->Front, this->WorldUp).normalize();
    this->Up    = crossVector(this->Right, this->Front).normalize();
}

void Camera::SetFocusPos(Vector3 fpos)
{
	Vector3 dir = (fpos - Position).normalize();
	if (dir == Vector3(0.f))
		return;
	this->Front = dir;

	this->Pitch = toTheta(::asinf(dir.y));
	if (this->Pitch > 89.0f)
		this->Pitch = 89.0f;
	if (this->Pitch < -89.0f)
		this->Pitch = -89.0f;

	if (fequal(dir.x, 0.f))
	{
		if (fequal(dir.z, 0.f))
			this->Yaw = 0.f;
		else if (dir.z > 0.f)
			this->Yaw = 90.f;
		else
			this->Yaw = -90.f;
	}
	else if (dir.x > 0.f)
	{
		this->Yaw = toTheta(::atanf(dir.z / dir.x));
	}
	else
	{
		if (dir.z >= 0.f)
			this->Yaw = 180.f + toTheta(::atanf(dir.z / dir.x));
		else
			this->Yaw = -180.f + toTheta(::atanf(dir.z / dir.x));
	}

	// re-calculate the Right and Up vector
	this->Right = crossVector(this->Front, this->WorldUp).normalize();
	this->Up = crossVector(this->Right, this->Front).normalize();
}