#include "Camera.h"

#include <vector>

#include "Quaternion.h"

//frustum
const float NEAR_CLIP = 0.1f;
const float FAR_CLIP = 1000.f;
const float FOV = 45.0f;
const float VIEW_PORT_WIDTH = 1280.f;
const float ASPECT = 1.777778f;

namespace myEngine
{
    Camera::Camera()
    : m_position(Vector3::UNIT_Y)
    , m_front(-Vector3::UNIT_Y)
    , m_rotate(0.f)
    , m_nearClip(NEAR_CLIP)
    , m_farClip(FAR_CLIP)
    , m_fov(FOV)
    , m_aspect(ASPECT)
    , m_viewProtWidth(VIEW_PORT_WIDTH)
    {

    }
    
    Camera::Camera(const Vector3& pos, const Vector3& focusPos)
    : m_position(pos)
    , m_front(focusPos - pos)
    , m_rotate(0.f)
    , m_nearClip(NEAR_CLIP)
    , m_farClip(FAR_CLIP)
    , m_fov(FOV)
    , m_aspect(ASPECT)
    , m_viewProtWidth(VIEW_PORT_WIDTH)
    {

    }
    
    Matrix4 Camera::GetViewMatrix() const
    {
        Matrix4 result;
        result.initWithLookAt(m_position, m_position + m_front, _getRight());
        return result;
    }
    
    Matrix4 Camera::GetProjectMatrix() const
    {
        Matrix4 result;
        result.initWithPerspective(getFov(), getAspect(), getNearClip(), getFarClip());
        return result;
    }
    
    void Camera::SetFocusPos(Vector3 fpos)
    {
        Vector3 dir = (fpos - m_position).normalize();
        if (dir == Vector3(0.f))
            return;
        m_front = dir;
    }

    void Camera::moveUp(float dis)
    {
        m_position += _getUp() * dis;
    }

    void Camera::moveRight(float dis)
    {
        m_position += _getRight() * dis;
    }

    void Camera::moveFront(float dis)
    {
        m_position += m_front * dis;
    }

    void Camera::rotateRight(float theta)
    {
        Quaternion _r;
        _r.setToRotateAboutAxis(_getUp(), -theta);
        _r.rotateVec3(m_front, m_front);
    }

    void Camera::rotateUp(float theta)
    {
        Quaternion _r;
        _r.setToRotateAboutAxis(_getRight(), theta);
        _r.rotateVec3(m_front, m_front);
    }

    Vector3 Camera::_getRight() const
    {
        Vector3 _right = crossVector(m_front, Vector3::UNIT_Y);
        Quaternion _rotate;
        _rotate.setToRotateAboutAxis(m_front, m_rotate);
        Vector3 _realRight = _right;
        _rotate.rotateVec3(_realRight, _right);
        _realRight.normalize();
        return _realRight;
    }

    Vector3 Camera::_getUp() const
    {
        return crossVector(_getRight(), m_front).normalize();
    }

}
