#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;

	float mYaw;
	float mPitch;

	float mWalkSpeed, mSprintSpeed;
	float mMovementSpeed;
	float mMouseSensitivity;
	float mFOV;

	float mLastMouseX;
	float mLastMouseY;
	bool bFirstMouse;

public:
	Camera(const glm::vec3 &position = glm::vec3(0.0f, 40.0f, 50.0f),
		const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = -90.0f,
		float pitch = -20.0f);

	// Movement
	void MoveForward(float deltaTime);
	void MoveBackward(float deltaTime);
	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);
	void MoveUp(float deltaTime);
	void MoveDown(float deltaTime);

	// Mouse look
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void ProcessMousePosition(float xPos, float yPos);

	// Getters
	float GetWalkSpeed() const { return mWalkSpeed; }
	float GetSprintSpeed() const { return mSprintSpeed; }
	float GetMovementSpeed() const { return mMovementSpeed; }
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix(float aspectRatio, float nearPlane = 0.1f, float farPlane = 1000.0f) const;
	const glm::vec3 &GetPosition() const { return mPosition; }
	const glm::vec3 &GetFront() const { return mFront; }
	float GetFOV() const { return mFOV; }

	// Setters
	void SetPosition(const glm::vec3 &position) { mPosition = position; UpdateCameraVectors(); }
	void SetMovementSpeed(const float speed) { mMovementSpeed = speed; }
	void SetWalkSpeed(const float speed) { mWalkSpeed = speed; }
	void SetSprintSpeed(const float speed) { mSprintSpeed = speed; }
	void SetMouseSensitivity(const float sensitivity) { mMouseSensitivity = sensitivity; }

private:
	void UpdateCameraVectors();
};