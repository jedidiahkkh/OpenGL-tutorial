#pragma once

#include "camera.h"

class FlyCamera : public Camera {
public:
  FlyCamera(
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
    float yaw = YAW,
    float pitch = PITCH
  ) : Camera(
    position,
    up,
    yaw,
    pitch
  ) {
    updateCameraVectors();
  }

  virtual void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;
    pitch = fmin(90.0f, fmax(-90.0f, pitch));

    updateCameraVectors();
  };
protected:
  virtual void updateCameraVectors() {
    if (pitch > 89.0f || pitch < -89.0f) {
      float sign = pitch > 0 ? 1.0f : -1.0f;
      glm::vec3 front;
      front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
      front.y = sin(glm::radians(pitch));
      front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
      this->front = glm::normalize(front);

      glm::vec3 azimuth;
      azimuth.x = cos(glm::radians(yaw));
      azimuth.y = 0.0f;
      azimuth.z = sin(glm::radians(yaw));
      right = sign * glm::cross(azimuth, front);
      right = glm::normalize(right);
    }
    else {
      glm::vec3 front;
      front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
      front.y = sin(glm::radians(pitch));
      front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
      this->front = glm::normalize(front);

      this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    }
    this->up = glm::normalize(glm::cross(this->right, this->front));
  }

};