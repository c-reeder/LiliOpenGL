#ifndef CAMERA_2D_H
#define CAMERA_2D_H
#include <glm/glm.hpp>

class Camera_2D
{
	public:
		Camera_2D(float displayWidth, float displayHeight);

		float displayWidth;
		float displayHeight;

		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();
	private:
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
};
#endif
