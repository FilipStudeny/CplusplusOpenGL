#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Camera {

private:

	float movementSpeed;
	float nearPlane;
	float farPlane;
	float FOV;
	float mouseSensitivity;

	float yaw;
	float pitch;
	
	float width;
	float height;

public: 
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraForwardDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUpDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	
	Camera(int width, int height);

	//****************
	//	SETTERS
	//****************
	 void SetFOV(float fov);
	 void SetNearPlane(float nearPlane);
	 void SetFarPlane(float farPlane);
	 void SetMouseSensitivity(float mouseSensitivity);
	 void SetMovementSpeed(float movementSpeed);
	 void SetCameraPosition(float x, float y, float z);
	 void SetCameraPosition(glm::vec3 cameraPosition);
	 void SetCameraForwardDire(glm::vec3 cameraForwardDirection);

	//****************
	//	GETTERS
	//****************
	 float GetAspectRatio();
	 float GetFOV();
	 float GetNearPlane();
	 float GetFarPlane();
	 float GetMovementSpeed();

	 glm::vec3 GetCameraPosition();
	 glm::vec3 GetCameraForwardDirection();
	 glm::vec3 GetCameraUpDirection();

	 glm::mat4 GetViewMatrix();

	//****************
	//	CALLS
	//****************
	 void KeyboardInput(GLFWwindow* window, float deltaTime);
	 void SetupCallbacks();

	//****************
	//	CALLBACKS
	//****************
	 void MousePositionUpdate(float xoffset, float  yoffset);
	 void MouseScrollUpdate(float yoffset);
	 

};