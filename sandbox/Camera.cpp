#define CAMERA_HPP

#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
	mEye = glm::vec3(0.0f, 0.0f, 0.0f);
	mViewDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	mUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 GetViewMatrix() const {
	return glm::lookAt(mEye, mViewDirection, mUpVector);
}


void Camera::MoveForward();
void Camera::MoveBackwards();
void Camera::MoveLeft();
void Camera::MoveRight();