#include "element/camera.h"

namespace mixi
{

const glm::vec3 Camera::WorldUp_ = glm::vec3(0.0f, 1.0f, 0.0f);
const float Camera::MovementSpeed_ = 2.5f;
const float Camera::MouseSensitivity_ = 0.1f;
const float Camera::PitchMax_ = 89.0f;
const float Camera::PitchMin_ = -89.0f;

Camera::Camera(glm::vec3 position) :
    position_(position),
    front_(),
    up_(),
    right_(),
    yaw_(-90.0f),
    pitch_(0.0f)
{
    updateCameraVectors();
}

Camera::~Camera()
{
    
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position_, position_ + front_, up_);
}

glm::vec3 Camera::getViewPoint() const
{
    return position_;
}

void Camera::moveForward(float delta)
{
    position_ += front_ * delta;
}

void Camera::moveBackWard(float delta)
{
    moveForward(-delta);
}

void Camera::moveRight(float delta)
{
    position_ += right_ * delta;
}

void Camera::moveLeft(float delta)
{
    moveRight(-delta);
}

void Camera::moveUp(float delta)
{
    position_ += up_ * delta;
}

void Camera::moveDown(float delta)
{
    moveUp(-delta);
}

void Camera::yawLeft(float delta)
{
    yaw_ -= delta;
    updateCameraVectors();
}

void Camera::yawRight(float delta)
{
    yaw_ += delta;
    updateCameraVectors();
}

void Camera::pitchUp(float delta)
{
    pitch_ += delta;
    if (pitch_ > PitchMax_) {
        pitch_ = PitchMax_;
    }
    updateCameraVectors();
}

void Camera::pitchDown(float delta)
{
    pitch_ -= delta;
    if (pitch_ < PitchMin_) {
        pitch_ = PitchMin_;
    }
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    right_ = glm::normalize(glm::cross(front_, WorldUp_));
    up_    = glm::normalize(glm::cross(right_, front_));
}

} // namespace mixi
