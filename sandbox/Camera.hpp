#ifndef CAMERA_H
#define CAMERA_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>


class Camera {
public:
	Camera();

	glm::mat4 GetViewMatrix() const;
	glm::vec2 OldMousePosition;

	glm::vec3 getPosition();

	void MouseLook(double mouseX, double mouseY);
	void MoveForward(float speed);
	void MoveBackward(float speed);
	void MoveLeft(float speed);
	void MoveRight(float speed);
	void MoveUp(float speed);
	void MoveDown(float speed);

private: 
	glm::vec3 mEye;
	glm::vec3 mViewDirection;
	glm::vec3 mUpVector;
	glm::vec3 mRightVector;

};

#endif