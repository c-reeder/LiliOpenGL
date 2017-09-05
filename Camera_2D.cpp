#include "Camera_2D.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera_2D::Camera_2D(float displayWidth, float displayHeight, float worldLength) : xpos(0.0f)
{
	this->displayWidth = displayWidth;
	this->displayHeight = displayHeight;
	this->worldLength = worldLength;

	projectionMatrix = glm::ortho(0.0f, displayWidth, displayHeight, 0.0f, -1.0f, 1.0f);  
}

void Camera_2D::setSize(int width, int height)
{
	projectionMatrix = glm::ortho(0.0f, (float) width, (float) height, 0.0f, -1.0f, 1.0f);
}

glm::mat4 Camera_2D::getViewMatrix()
{
	glm::mat4 viewMatrix;
	viewMatrix = glm::translate(viewMatrix, glm::vec3(-xpos, 0.0f, 0.0f));
	return viewMatrix;
}
glm::mat4 Camera_2D::getProjectionMatrix()
{
	return projectionMatrix;
}

float Camera_2D::midX()
{
	return xpos + (displayWidth / 2.0f);
}

void Camera_2D::centerOn(float midX)
{
	float potX = midX - (displayWidth / 2.0f);
	if (potX < 0.0f)
		potX = 0.0f;
	if (potX > worldLength - displayWidth)
		potX = worldLength - displayWidth;
	xpos = potX;
}
