#ifndef OBJECT_H
#define OBJECT_H
#include "Shader.hpp"
#include "Camera_2D.hpp"

class Object
{
	public:
		Object(Camera_2D* camera, char const* imagePath, int textureUnit);
		void draw();

		float height;
		float width;
		glm::vec2 position;

	private:
		Shader shader;
		unsigned int VAO;
		int textureUnit;
		Camera_2D* camera;
		static float vertices[];
		int texture;
};
#endif
