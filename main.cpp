#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.hpp"
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
glm::vec2 position(0.0f, 3.0f * (screenHeight / 4.0f) - 250.0f);

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	GLFWwindow* window =
		glfwCreateWindow(screenWidth,screenHeight,"LearnOpenGL", NULL,NULL);
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

	Shader spriteShader("spriteVert.glsl", "spriteFrag.glsl");

    GLfloat vertices[] = { 
        // Pos      // Tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

	// Create Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
			vertices, GL_STATIC_DRAW);

	// Create/Setup Box VAO
	unsigned int spriteVAO;
	glGenVertexArrays(1, &spriteVAO);
	glBindVertexArray(spriteVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	int spriteTextures[4];
	spriteTextures[0] = loadTexture("res/LiliRunRight/lilirunright1.png");
	spriteTextures[1] = loadTexture("res/LiliRunRight/lilirunright2.png");
	spriteTextures[2] = loadTexture("res/LiliRunRight/lilirunright3.png");
	spriteTextures[3] = loadTexture("res/LiliRunRight/lilirunright4.png");
	int grassTexture = loadTexture("res/grass.png");

	/**
	 * Textures
	 * ------------------------
	 * 0 - Sprite Image
	 * 1 - Grass Image
	 *
	 */

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, spriteTextures[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grassTexture);

	float spriteLastFrame;
	int spriteLastTexture;
	while(!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		//printf("currentFrame: %f, spriteLastFrame: %f\n", currentFrame, spriteLastFrame);
		if (currentFrame - spriteLastFrame > 0.15f) {
			glActiveTexture(GL_TEXTURE0);
			spriteLastTexture = (spriteLastTexture + 1) % 4;
			glBindTexture(GL_TEXTURE_2D, spriteTextures[spriteLastTexture]);
			spriteLastFrame = currentFrame;
		}
		
		processInput(window);

		glClearColor(0.82f, 0.52f, 0.93f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Begin Using Shader
		spriteShader.use();

		// Create model matrix
		glm::mat4 spriteModel;
		spriteModel = glm::translate(spriteModel, glm::vec3(position, 0.0f));
		spriteModel = glm::scale(spriteModel, glm::vec3(400.0f, 250.0f, 1.0f));

		// Send sprite transformation matrices to Graphics Card
		spriteShader.setMatrix4fv("model", spriteModel);
		spriteShader.setMatrix4fv("projection", projection);
		spriteShader.set1i("image", 0);
		
		// Draw Sprite
		glBindVertexArray(spriteVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glm::mat4 grassModel;
		grassModel = glm::translate(grassModel, glm::vec3(0.0f, 3.0f * (screenHeight / 4.0f), 0.0f));
		grassModel = glm::scale(grassModel,
				glm::vec3(screenWidth, (screenHeight / 4.0f),
					1.0f));

		spriteShader.setMatrix4fv("model", grassModel);
		spriteShader.setMatrix4fv("projection", projection);
		spriteShader.set1i("image", 1);

		// Draw Grass
		glBindVertexArray(spriteVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

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
		position.x -= deltaTime * 1000;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		position.x += deltaTime * 1000;
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SOIL_free_image_data(data);
	return textureID;
}
