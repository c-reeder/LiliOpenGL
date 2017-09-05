#ifndef CAMERA_2D_H
#define CAMERA_2D_H
#include <glm/glm.hpp>

class Camera_2D
{
	public:
		Camera_2D(float displayWidth, float displayHeight,
				float worldLength);

		float displayWidth;
		float displayHeight;
		float worldLength;

		float xpos;


		void setSize(int width, int height);

		void centerOn(float midX);

		float midX();

		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();
	private:
		glm::mat4 projectionMatrix;
};
#endif
