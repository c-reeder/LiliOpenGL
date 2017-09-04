/**
 * A class which represents a single object within the game.
 */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Object.hpp"

static unsigned int loadTexture(char const * path, int* imgWidth, int* imgHeight);

float Object::vertices[] = { 
	// Pos      // Tex
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 

	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f
};

Object::Object(char const* imagePath, int textureUnit) : shader("spriteVert.glsl", "spriteFrag.glsl"), position(0.0f, 0.0f) 
{
	this->textureUnit = textureUnit;

	// Create Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Object::vertices),
			Object::vertices, GL_STATIC_DRAW);

	// Create Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// Load Image Texture
	int imgWidth;
	int imgHeight;
	texture = loadTexture(imagePath, &imgWidth, &imgHeight);

	// Set Default dimmensions of Object to the dimensions of image
	this->width = imgWidth;
	this->height = imgHeight;

	int activeUnit = GL_TEXTURE0 + textureUnit;
	glActiveTexture(activeUnit);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader.use();
}

void Object::draw(glm::mat4 projection)
{
	float currentFrame = glfwGetTime();
	shader.use();

	// Activate Texture
	int activeUnit = GL_TEXTURE0 + textureUnit;
	glActiveTexture(activeUnit);
	glBindTexture(GL_TEXTURE_2D, texture);

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(glm::vec3(width, height, 1.0f)));


	shader.setMatrix4fv("model", model);
	shader.setMatrix4fv("projection", projection);
	shader.set1i("image", textureUnit);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

}


static unsigned int loadTexture(char const * path, int* imgWidth, int* imgHeight)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	unsigned char* data = SOIL_load_image(path,
			imgWidth, imgHeight, 0, SOIL_LOAD_RGBA);
	if( 0 == data )
	{
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
	}


	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *imgWidth, *imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SOIL_free_image_data(data);
	return textureID;
}
