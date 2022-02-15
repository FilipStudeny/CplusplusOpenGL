#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

Camera::Camera(int width, int height) {
	

	mouseSensitivity = 0.1f;
	movementSpeed = 2.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;
	FOV = 45.0f;

	yaw = -90.0f; //-90 TO POINT CAMERA FORWARD
	pitch = 0.0f;

	this->width = width;
	this->height = height;

}

//****************
//	SETTERS
//****************
void Camera::SetFOV(float fov) { this->FOV = fov; }
void Camera::SetNearPlane(float nearPlane) { this->nearPlane = nearPlane; }
void Camera::SetFarPlane(float farPlane) { this->farPlane = farPlane; }
void Camera::SetMouseSensitivity(float mouseSensitivity) { this->mouseSensitivity = mouseSensitivity; }
void Camera::SetMovementSpeed(float movementSpeed) { this->movementSpeed = movementSpeed; }
void Camera::SetCameraPosition(float x, float y, float z) { this->cameraPosition = glm::vec3(x, y, z); }
void Camera::SetCameraPosition(glm::vec3 cameraPosition) { this->cameraPosition = cameraPosition; }
void Camera::SetCameraForwardDire(glm::vec3 cameraForwardDirection) { this->cameraForwardDirection = cameraForwardDirection; }

//****************
//	GETTERS
//****************
float Camera::GetAspectRatio() { return (float)this->width / (float)this->height; }
float Camera::GetFOV() { return this->FOV; }
float Camera::GetNearPlane() { return this->nearPlane; }
float Camera::GetFarPlane() { return this->farPlane; }
float Camera::GetMovementSpeed() { return this->movementSpeed; }

glm::vec3 Camera::GetCameraPosition() { return this->cameraPosition; }
glm::vec3 Camera::GetCameraForwardDirection() { return this->cameraForwardDirection; }
glm::vec3 Camera::GetCameraUpDirection() { return this->cameraUpDirection; }

glm::mat4 Camera::GetViewMatrix() { return glm::lookAt(cameraPosition, cameraPosition + cameraForwardDirection, cameraUpDirection); }

//****************
//	CALLS
//****************
void Camera::KeyboardInput(GLFWwindow* window, float deltaTime) {

	//KEYBOARD INPUT
	float cameraStep =  this->movementSpeed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPosition += cameraStep * cameraForwardDirection;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPosition -= cameraStep * cameraForwardDirection;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPosition -= glm::normalize(glm::cross(cameraForwardDirection, cameraUpDirection)) * cameraStep;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPosition += glm::normalize(glm::cross(cameraForwardDirection, cameraUpDirection)) * cameraStep;

}

//****************
//	CALLBACKS
//****************
void Camera::MousePositionUpdate(float xoffset, float  yoffset) {

	xoffset *= this->mouseSensitivity;
	yoffset *= this->mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	//CLAMP VERTICAL ROTATION
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraForwardDirection = glm::normalize(front);

}

void Camera::MouseScrollUpdate(float yoffset) {
	this->FOV -= yoffset;
	if (this->FOV < 1.0f)
		this->FOV = 1.0f;
	if (this->FOV > 45.0f)
		this->FOV = 45.0f;
}