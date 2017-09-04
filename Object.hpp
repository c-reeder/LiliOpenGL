#ifndef OBJECT_H
#define OBJECT_H
#include "Shader.hpp"

class Object
{
	public:
		Object(char const* imagePath, int textureUnit);
		void draw(glm::mat4 projection);

		float height;
		float width;
		glm::vec2 position;

	private:
		Shader shader;
		unsigned int VAO;
		int textureUnit;
		static float vertices[];
		int texture;
};
#endif
