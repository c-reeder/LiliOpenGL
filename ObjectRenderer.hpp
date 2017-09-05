#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H
#include "Camera_2D.hpp"
#include "Shader.hpp"

class ObjectRenderer
{
	public:
		ObjectRenderer(Camera_2D* camera,
				char const* imgPath, int textureUnit);

	private:
		int textureUnit;
		Shader shader;
		unsigned int VAO;
		Camera_2D* camera;
		static float vertices[];
		int texture;
};


#endif
