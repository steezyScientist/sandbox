#include "Camera.hpp"

Camera::Camera() {
	mEye = glm::vec3(0.0f, 0.0f, 3.0f);
	mViewDirection = glm::vec3(0.0f, 0.0f, -3.0f);
	mUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(mEye, mEye + mViewDirection, mUpVector);
}

glm::vec3 Camera::getPosition(){
	return mEye;
}

void Camera::MouseLook(double mouseX, double mouseY) {



	glm::vec2 currentMouse = glm::vec2(mouseX, mouseY);

	static bool firstLook = true;
	if (firstLook) {
		OldMousePosition = currentMouse;
		firstLook = false;
	}

	glm::vec2 mouseDelta = OldMousePosition - currentMouse;

	//horizontal rotation (YAW)
	mViewDirection = glm::rotate(mViewDirection, glm::radians(mouseDelta.x), mUpVector);

	// Vertical rotation (PITCH) around the right vector
	glm::vec3 rightVector = glm::cross(mUpVector, mViewDirection);
	mViewDirection = glm::rotate(mViewDirection, glm::radians(mouseDelta.y), -rightVector);

	OldMousePosition = currentMouse;
}

void Camera::MoveForard(float speed) {
	mEye += (speed * mViewDirection);
}
void Camera::MoveBackward(float speed) {
	mEye -= (speed * mViewDirection);
}
void Camera::MoveLeft(float speed) {
	glm::vec3 rightVector = glm::cross(mViewDirection, mUpVector);
	mEye -= rightVector * speed;
}
void Camera::MoveRight(float speed) {
	glm::vec3 rightVector = glm::cross(mViewDirection, mUpVector);
	mEye += rightVector * speed;
}
void Camera::MoveUp(float speed) {
	mEye.y += speed * 2.0f;
}
void Camera::MoveDown(float speed) {
	mEye.y -= speed * 2.0f;
}