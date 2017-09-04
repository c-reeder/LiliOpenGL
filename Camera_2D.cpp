#include "Camera_2D.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera_2D::Camera_2D(float displayWidth, float displayHeight)
{
	this->displayWidth = displayWidth;
	this->displayHeight = displayHeight;

	projectionMatrix = glm::ortho(0.0f, displayWidth, displayHeight, 0.0f, -1.0f, 1.0f);  
}

void Camera_2D::setSize(int width, int height)
{
	projectionMatrix = glm::ortho(0.0f, (float) width, (float) height, 0.0f, -1.0f, 1.0f);
}

glm::mat4 Camera_2D::getViewMatrix()
{
	return viewMatrix;
}
glm::mat4 Camera_2D::getProjectionMatrix()
{
	return projectionMatrix;
}
