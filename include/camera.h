#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  ALT_LEFT,
  ALT_RIGHT
};

// default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float ROLL = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  float yaw;
  float pitch;
  float roll;
  float movementSpeed;
  float mouseSensitivity;
  float zoom;

  Camera(
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
    float yaw = YAW,
    float pitch = PITCH,
    float roll = ROLL
  ) :
    position(position),
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    worldUp(up),
    yaw(yaw),
    pitch(pitch),
    movementSpeed(SPEED),
    mouseSensitivity(SENSITIVITY),
    zoom(ZOOM)
  {
    // updateCameraVectors();
  };

  glm::mat4 getViewMatrix() {
    return glm::lookAt(position, position + front, up);
  }

  virtual void processKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    switch (direction) {
    case FORWARD:
      position += front * velocity;
      break;
    case BACKWARD:
      position -= front * velocity;
      break;
    case LEFT:
      position -= right * velocity;
      break;
    case RIGHT:
      position += right * velocity;
      break;
    }
  };

  virtual void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) = 0;
  virtual void processMouseScroll(float yoffset) {
    zoom -= yoffset;
    zoom = fmin(90.0f, fmax(1.0f, zoom));
  }

protected:
  // calculates vectors from the Camera's euler angles
  virtual void updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);

    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
  }
};