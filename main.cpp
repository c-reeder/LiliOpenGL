#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.hpp"
#include "Sprite.hpp"
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
unsigned int loadTexture(char const * path);

float screenWidth = 800.0f;
float screenHeight = 600.0f;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::mat4 projection = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);  

Sprite* sprite = NULL;

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	GLFWwindow* window =
		glfwCreateWindow(screenWidth,screenHeight,"Lili Game", NULL,NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0,0,screenWidth,screenHeight);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	Shader grassShader("spriteVert.glsl", "spriteFrag.glsl");

	// Initialize Sprite
	sprite = new Sprite(0);
	sprite->height = 250.0f / 2.0f;
	sprite->width = 400.0f / 2.0f;
	sprite->position = 
		glm::vec2(0.0f, 3.0f * (screenHeight / 4.0f) - sprite->height);

	float texWide = screenWidth / 32.0f;
	float texHigh = (screenHeight / 4.0f) / 32.0f;

	printf("texWide: %f, texHigh: %f\n", texWide, texHigh);

	GLfloat grassVertices[] = {
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, texHigh,
        1.0f, 0.0f, texWide, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, texHigh,
        1.0f, 1.0f, texWide, texHigh,
        1.0f, 0.0f, texWide, 0.0f
	};

	// Create Vertex Buffer Objects
	unsigned int grassVBO;
	glGenBuffers(1, &grassVBO);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices),
			grassVertices, GL_STATIC_DRAW);

	// Create/Setup VAOs
	unsigned int grassVAO;
	glGenVertexArrays(1, &grassVAO);
	glBindVertexArray(grassVAO);

	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	// Load Textures

	/**
	 * Textures
	 * ------------------------
	 * 0 - Sprite Image
	 * 1 - Grass Image
	 *
	 */

	glActiveTexture(GL_TEXTURE1);
	int grassTexture = loadTexture("res/grass.png");
	glBindTexture(GL_TEXTURE_2D, grassTexture);

	while(!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.82f, 0.52f, 0.93f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		grassShader.use();

		glm::mat4 grassModel;
		grassModel = glm::translate(grassModel, glm::vec3(0.0f, 3.0f * (screenHeight / 4.0f), 0.0f));
		grassModel = glm::scale(grassModel,
				glm::vec3(screenWidth, (screenHeight / 4.0f),
					1.0f));

		grassShader.setMatrix4fv("model", grassModel);
		grassShader.setMatrix4fv("projection", projection);
		grassShader.set1i("image", 1);

		// Draw Grass
		glBindVertexArray(grassVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		sprite->draw(projection);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	delete sprite;

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0,0,width, height);
	screenWidth = width;
	screenHeight = height;
	projection = glm::ortho(0.0f, (float) width, (float) height, 0.0f, -1.0f, 1.0f);
}

void processInput(GLFWwindow *window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		//position.y -= deltaTime * 1000;
	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		//position.y += deltaTime * 1000;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		sprite->position.x -= deltaTime * 1000;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		sprite->position.x += deltaTime * 1000;
}

unsigned int loadTexture(char const * path)
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SOIL_free_image_data(data);
	return textureID;
}
