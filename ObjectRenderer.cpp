#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ObjectRenderer.hpp"

static unsigned int loadTexture(char const * path);

float ObjectRenderer::vertices[] = { 
	// Pos      // Tex
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 

	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f
};

ObjectRenderer::ObjectRenderer(Camera_2D* camera,
		char const* imgPath, int textureUnit) : shader("objectVert.glsl", "objectFrag.glsl")
{
	this->camera = camera;
	this->textureUnit = textureUnit;

	// Create Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
			vertices, GL_STATIC_DRAW);

	// Create Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// Load Image Texture
	texture = loadTexture(imgPath);

	int activeUnit = GL_TEXTURE0 + textureUnit;
	glActiveTexture(activeUnit);
	glBindTexture(GL_TEXTURE_2D, texture);

	shader.use();
	shader.set1i("image", textureUnit);
}

void ObjectRenderer::drawObject(float xPos, float yPos, float width, float height)
{
	shader.use();
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(xPos, yPos, 0.0f));
	model = glm::scale(model, glm::vec3(width, height, 1.0f));

	shader.setMatrix4fv("model", model);
	shader.setMatrix4fv("view", camera->getViewMatrix());
	shader.setMatrix4fv("projection", camera->getProjectionMatrix());

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

static unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int imgWidth, imgHeight;
	unsigned char* data = SOIL_load_image(path,
			&imgWidth, &imgHeight, 0, SOIL_LOAD_RGBA);
	if( 0 == data )
	{
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
	}


	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SOIL_free_image_data(data);
	return textureID;
}
