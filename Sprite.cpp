/**
 * A class which represents a single sprite (character) within the game.
 */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include <SOIL/SOIL.h>
#include "Sprite.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static unsigned int loadTexture(char const * path);

float Sprite::vertices[] = { 
	// Pos      // Tex
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 

	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f
};

Sprite::Sprite(int textureUnit) : shader("spriteVert.glsl", "spriteFrag.glsl"), position(0.0f, 0.0f), width(400.0f), height(250.0f), lastTextureIdx(0)
{
	this->textureUnit = textureUnit;
	printf("Init Sprite with textureUnit: %d\n", textureUnit);

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

	// Load Textures
	textures[0] = loadTexture("res/LiliRunRight/lilirunright1.png");
	textures[1] = loadTexture("res/LiliRunRight/lilirunright2.png");
	textures[2] = loadTexture("res/LiliRunRight/lilirunright3.png");
	textures[3] = loadTexture("res/LiliRunRight/lilirunright4.png");


	int activeUnit = GL_TEXTURE0 + textureUnit;
	printf("first activeUnit: %d, lastTextureIdx: %d, textures[lastTextureIdx]: %d\n", activeUnit, lastTextureIdx, textures[lastTextureIdx]);
	glActiveTexture(activeUnit);
	glBindTexture(GL_TEXTURE_2D, textures[lastTextureIdx]);
}

void Sprite::draw(glm::mat4 projection)
{
	float currentFrame = glfwGetTime();

		//printf("currentFrame: %f, lastTextureSwitch: %f\n", currentFrame, lastTextureSwitch);
	if (currentFrame - lastTextureSwitch > 0.15f) {
		int activeUnit = GL_TEXTURE0 + textureUnit;
		printf("activeUnit: %d, lastTextureIdx: %d, textures[lastTextureIdx]: %d\n", activeUnit, lastTextureIdx, textures[lastTextureIdx]);
		glActiveTexture(activeUnit);
		lastTextureIdx = (lastTextureIdx + 1) % 4;
		glBindTexture(GL_TEXTURE_2D, textures[lastTextureIdx]);
		lastTextureSwitch = currentFrame;
	}

	shader.use();

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(glm::vec3(width, height, 1.0f)));


	shader.setMatrix4fv("model", model);
	shader.setMatrix4fv("projection", projection);
	shader.set1i("image", textureUnit);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

}


static unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* data = SOIL_load_image(path,
			&width, &height, 0, SOIL_LOAD_RGBA);
	if( 0 == data )
	{
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
	}

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SOIL_free_image_data(data);
	return textureID;
}
