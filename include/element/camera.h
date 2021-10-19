#pragma once

#include "pch.h"

namespace mixi
{

class Camera
{
public:

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));
    ~Camera();

    glm::mat4 getViewMatrix() const;

    void moveForward(float delta);
    void moveBackWard(float delta);
    void moveLeft(float delta);
    void moveRight(float delta);
    void moveUp(float delta);
    void moveDown(float delta);

    void yawLeft(float delta);
    void yawRight(float delta);
    void pitchUp(float delta);
    void pitchDown(float delta);

protected:

    // camera Attributes
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;

    // euler Angles
    float yaw_;
    float pitch_;

private:

    void updateCameraVectors();

    static const glm::vec3 WorldUp_;
    static const float MovementSpeed_;
    static const float MouseSensitivity_;
    static const float PitchMax_;
    static const float PitchMin_;
};

} // namespace mixi
