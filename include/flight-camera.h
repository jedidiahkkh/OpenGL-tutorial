#pragma once

#include "camera.h"

class FlightCamera : public Camera {
public:
  FlightCamera(
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
    float yaw = YAW,
    float pitch = PITCH,
    float roll = ROLL
  ) : Camera(
    position,
    up,
    yaw,
    pitch,
    roll
  ) {
    updateCameraVectors();
  }

  virtual void processKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    glm::mat4 yawRotate = glm::rotate(glm::mat4(1.0f), glm::radians(velocity * 10.0f), up);
    glm::vec4 newFront;
    switch (direction) {
    case FORWARD:
      position += front * velocity;
      break;
    case BACKWARD:
      position -= front * velocity;
      break;
    case LEFT:
      newFront = yawRotate * glm::vec4(front.x, front.y, front.z, 1.0f);
      front.x = newFront.x;
      front.y = newFront.y;
      front.z = newFront.z;
      right = glm::normalize(glm::cross(front, up));
      break;
    case RIGHT:
      newFront = glm::transpose(yawRotate) * glm::vec4(front.x, front.y, front.z, 1.0f);
      front.x = newFront.x;
      front.y = newFront.y;
      front.z = newFront.z;
      right = glm::normalize(glm::cross(front, up));
      break;

    }
  }

  virtual void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    glm::mat4 pitchRotate = glm::rotate(glm::mat4(1.0f), glm::radians(yoffset), right);
    front = glm::vec3(pitchRotate * glm::vec4(front.x, front.y, front.z, 1.0f));
    up = glm::vec3(pitchRotate * glm::vec4(up.x, up.y, up.z, 1.0f));
    glm::mat4 rollRotate = glm::rotate(glm::mat4(1.0f), glm::radians(xoffset), front);
    up = glm::normalize(glm::vec3(rollRotate * glm::vec4(up.x, up.y, up.z, 1.0f)));
    right = glm::normalize(glm::cross(front, up));
  };
};