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
#include "Camera_2D.hpp"

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

Sprite::Sprite(Camera_2D* camera, int textureUnit) : shader("spriteVert.glsl", "spriteFrag.glsl"), position(0.0f, 0.0f), width(400.0f), height(250.0f), lastTextureIdx(3), lastTextureSwitch(-0.15f)
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

	// Load Textures
	rightTextures[0] = loadTexture("res/LiliRunRight/lilirunright1.png");
	rightTextures[1] = loadTexture("res/LiliRunRight/lilirunright2.png");
	rightTextures[2] = loadTexture("res/LiliRunRight/lilirunright3.png");
	rightTextures[3] = loadTexture("res/LiliRunRight/lilirunright4.png");
	rightTextures[4] = loadTexture("res/LiliRunRight/lilistandright.png");

	leftTextures[0] = loadTexture("res/LiliRunLeft/lilirunleft1.png");
	leftTextures[1] = loadTexture("res/LiliRunLeft/lilirunleft2.png");
	leftTextures[2] = loadTexture("res/LiliRunLeft/lilirunleft3.png");
	leftTextures[3] = loadTexture("res/LiliRunLeft/lilirunleft4.png");
	leftTextures[4] = loadTexture("res/LiliRunLeft/lilistandleft.png");

	int activeUnit = GL_TEXTURE0 + textureUnit;
	glActiveTexture(activeUnit);
	glBindTexture(GL_TEXTURE_2D, rightTextures[4]);
	shader.use();
}

void Sprite::draw(float deltaPos)
{
	float currentFrame = glfwGetTime();
	shader.use();

	static float lastDeltaPos = 1.0f;

	//printf("currentFrame: %f, lastTextureSwitch: %f\n", currentFrame, lastTextureSwitch);
	if (deltaPos == 0.0f)
	{
		int activeUnit = GL_TEXTURE0 + textureUnit;
		glActiveTexture(activeUnit);
		lastTextureIdx = 3;
		if (lastDeltaPos > 0.0f)
			glBindTexture(GL_TEXTURE_2D, rightTextures[4]);
		else if (lastDeltaPos < 0.0f)
			glBindTexture(GL_TEXTURE_2D, leftTextures[4]);
	} else if (currentFrame - lastTextureSwitch > 0.15f) {
		int activeUnit = GL_TEXTURE0 + textureUnit;
		glActiveTexture(activeUnit);
		lastTextureIdx = (lastTextureIdx + 1) % 4;
		if (deltaPos > 0.0f)
			glBindTexture(GL_TEXTURE_2D, rightTextures[lastTextureIdx]);
		else if (deltaPos < 0.0f)
			glBindTexture(GL_TEXTURE_2D, leftTextures[lastTextureIdx]);
		lastTextureSwitch = currentFrame;
		lastDeltaPos = deltaPos;
	} else if (currentFrame < lastTextureSwitch) {
		std::cerr << "WHOAAA!!!" << std::endl;
	}

	shader.use();

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::scale(model, glm::vec3(glm::vec3(width, height, 1.0f)));


	shader.setMatrix4fv("model", model);
	shader.setMatrix4fv("view", camera->getViewMatrix());
	shader.setMatrix4fv("projection", camera->getProjectionMatrix());
	shader.set1i("image", textureUnit);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

}


float Sprite::midX()
{
	return position.x + (width / 2.0f);
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
