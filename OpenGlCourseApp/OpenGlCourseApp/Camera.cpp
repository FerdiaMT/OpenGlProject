#include "Camera.h"

Camera::Camera() 
{

}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

}

void Camera::keyControl(bool* keys)
{
	if (keys[GLFW_KEY_W])
	{
		position += front * moveSpeed;
	}
	if (keys[GLFW_KEY_S])
	{
		position -= front * moveSpeed;
	}
	if (keys[GLFW_KEY_A])
	{
		position -= right * moveSpeed;
	}
	if (keys[GLFW_KEY_D])
	{
		position += right * moveSpeed;
	}
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up); // position, where the thing is were looking at (our case whats in front) , where is up
	//we could easily make this third person by having a behind position camera looking at the person
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
Camera::~Camera()
{

}