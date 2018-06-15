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
const GLfloat SPEED      =  10.f;
const GLfloat SENSITIVTY =  0.1f;
const GLfloat ZOOM       =  45.0f;

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
    GLfloat Zoom;

    Camera(Vector3 position = Vector3(0.0f, 0.0f, 0.0f),
           Vector3 up = Vector3(0.0f, 1.0f, 0.0f),
           GLfloat yaw = YAW,
           GLfloat pitch = PITCH);

    Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
           GLfloat upX, GLfloat upY, GLfloat upZ,
           GLfloat yaw, GLfloat pitch);

    Matrix4 GetViewMatrix();

    void ProcessKeyboard(CameraMove direction, GLfloat deltaTime);

    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

    void ProcessMouseScroll(GLfloat yoffset);

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors();
};
#endif