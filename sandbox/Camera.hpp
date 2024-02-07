#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public: 

	Camera();

	glm::mat4 GetViewMatrix() const {
		return glm::lookAt(mEye, mViewDirection, mUpVector);
	}

	void MoveForward();
	void MoveBackwards();
	void MoveLeft();
	void MoveRight();

private:
	glm::vec3 mEye;
	glm::vec3 mViewDirection;
	glm::vec3 mUpVector;

};