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
	ROTATERIGHT
};

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  2.5f;
const GLfloat SENSITIVTY =  0.1f;
const GLfloat ZOOM       =  45.0f;

class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Eular Angles
    GLfloat Yaw;
    GLfloat Pitch;

    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
           GLfloat yaw = YAW, 
           GLfloat pitch = PITCH);

    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, 
           GLfloat upX, GLfloat upY, GLfloat upZ, 
           GLfloat yaw, GLfloat pitch);

    glm::mat4 GetViewMatrix();

    void ProcessKeyboard(CameraMove direction, GLfloat deltaTime);

    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

    void ProcessMouseScroll(GLfloat yoffset);

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors();
};
#endif