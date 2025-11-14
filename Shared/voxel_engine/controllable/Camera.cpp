#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3 &position, const glm::vec3 &up, const float yaw, const float pitch)
	: mPosition(position)
	, mWorldUp(up)
	, mYaw(yaw)
	, mPitch(pitch)
	, mWalkSpeed(4.3f)
	, mSprintSpeed(5.6f)
	, mMovementSpeed(10.0f)
	, mMouseSensitivity(0.1f)
	, mFOV(60.0f)
	, mLastMouseX(0.0f)
	, mLastMouseY(0.0f)
	, bFirstMouse(true) {
	UpdateCameraVectors();
}

void Camera::MoveForward(const float deltaTime) {
	mPosition += mFront * mMovementSpeed * deltaTime;
}

void Camera::MoveBackward(const float deltaTime) {
	mPosition -= mFront * mMovementSpeed * deltaTime;
}

void Camera::MoveLeft(const float deltaTime) {
	mPosition -= mRight * mMovementSpeed * deltaTime;
}

void Camera::MoveRight(const float deltaTime) {
	mPosition += mRight * mMovementSpeed * deltaTime;
}

void Camera::MoveUp(const float deltaTime) {
	mPosition += mWorldUp * mMovementSpeed * deltaTime;
}

void Camera::MoveDown(const float deltaTime) {
	mPosition -= mWorldUp * mMovementSpeed * deltaTime;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, const bool constrainPitch) {
	xOffset *= mMouseSensitivity;
	yOffset *= mMouseSensitivity;

	mYaw += xOffset;
	mPitch += yOffset;

	// Constrain pitch to prevent screen flip
	if (constrainPitch)
	{
		if (mPitch > 89.0f)
			mPitch = 89.0f;
		if (mPitch < -89.0f)
			mPitch = -89.0f;
	}

	UpdateCameraVectors();
}

void Camera::ProcessMousePosition(const float xPos, const float yPos) {
	if (bFirstMouse)
	{
		mLastMouseX = xPos;
		mLastMouseY = yPos;
		bFirstMouse = false;
	}

	const float xOffset = xPos - mLastMouseX;
	const float yOffset = mLastMouseY - yPos;  // Reversed: y-coordinates go from bottom to top

	mLastMouseX = xPos;
	mLastMouseY = yPos;

	ProcessMouseMovement(xOffset, yOffset);
}

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

glm::mat4 Camera::GetProjectionMatrix(const float aspectRatio, const float nearPlane, const float farPlane) const {
	return glm::perspective(glm::radians(mFOV), aspectRatio, nearPlane, farPlane);
}

void Camera::UpdateCameraVectors() {
	// Calculate new front vector
	glm::vec3 front;
	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(front);

	// Recalculate right and up vectors
	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mFront));
}