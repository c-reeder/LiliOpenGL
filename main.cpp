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
#include "Game.hpp"
#include "Object.hpp"
#include "Camera_2D.hpp"

using namespace std;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
unsigned int loadTexture(char const * path);

float screenWidth = 800.0f;
float screenHeight = 600.0f;

float levelLength = screenWidth * 20; // 16,000 for now


float deltaTime = 0.0f;
float deltaPos = 0.0f;
float lastFrame = 0.0f;

Camera_2D camera(screenWidth, screenHeight);

Sprite* sprite = NULL;
Object* clouds[5];


Game_State gameState;

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

	gameState = BW;

	// Initialize Sprite
	sprite = new Sprite(&camera, 0);
	sprite->height = 250.0f / 2.0f;
	sprite->width = 400.0f / 2.0f;
	sprite->position = 
		glm::vec2(screenWidth / 2.0f - sprite->width / 2.0f, 3.0f * (screenHeight / 4.0f) - sprite->height);

	// Initialize Clouds
	for (int i = 0; i < 5; i++)
	{

		clouds[i] = new Object(&camera, "res/cloud.png", 2);
		clouds[i]->height = 250.0f / 2.0f;
		clouds[i]->width = 400.0f / 2.0f;
		clouds[i]->position.x = i * 300.0f;

	}
	//cloud = new Object(&camera, "res/cloud.png", 2);
	//cloud->height = 250.0f / 2.0f;
	//cloud->width = 400.0f / 2.0f;
	//cloud->position.x = 200.0f;


	float texWide = screenWidth / 32.0f;
	float texHigh = (screenHeight / 4.0f) / 32.0f;

	//printf("texWide: %f, texHigh: %f\n", texWide, texHigh);

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

	bool firstTime = true;

	printf("whoa: sprite->position.x: %f, (camera.xpos + (screenWidth / 5.0f)): %f\n", sprite->position.x, (camera.xpos + (screenWidth / 5.0f)));

	//camera.xpos = 435.0f;
	while(!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		processInput(window);

		glClearColor(0.82f, 0.52f, 0.93f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/**
		 *
		 * Add static Grass
		 *
		 */
		grassShader.use();
		// Setup Grass Model Matrix
		glm::mat4 grassModel;
		grassModel = glm::translate(grassModel, glm::vec3(0.0f, 3.0f * (screenHeight / 4.0f), 0.0f));
		grassModel = glm::scale(grassModel,
				glm::vec3(screenWidth, (screenHeight / 4.0f),
					1.0f));
		// Send Grass transform matrices
		grassShader.setMatrix4fv("model", grassModel);
		glm::mat4 stationaryView;
		grassShader.setMatrix4fv("view", stationaryView);
		grassShader.setMatrix4fv("projection",
				camera.getProjectionMatrix());
		grassShader.set1i("image", 1);
		// Draw Grass
		glBindVertexArray(grassVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		
		// Update Camera Position
		if (gameState == BW && deltaPos <= 0)
		{
			//if (camera.xpos - sprite->position.x > deltaPos * 2) {
				//printf("1\n");
				//camera.xpos = max(camera.xpos - 3000 * deltaTime,
						//sprite->position.x);
			//} else if (camera.xpos - sprite->position.x > 0) {
				//printf("2\n");
				//camera.xpos = sprite->position.x;
			//}

			if (camera.midX() - sprite->midX() > deltaPos * -2.0f) {
				printf("1\n");
				printf("camera.midX(): %f, sprite->midX(): %f\n", camera.midX(), sprite->midX());
				printf("camera.midX() - sprite->midX(): %f, deltaPos: %f\n\n", camera.midX() - sprite->midX(), deltaPos);
				//camera.xpos = max(camera.xpos - 3000 * deltaTime,
						//sprite->position.x);
				camera.centerOn(max(camera.midX() - 3000 * deltaTime,
						sprite->midX()));
			} else if (camera.midX() - sprite->midX() > 0) {
				printf("2\n");
				printf("camera.midX(): %f, sprite->midX(): %f\n\n", camera.midX(), sprite->midX());
				printf("camera.midX() - sprite->midX(): %f, deltaPos: %f\n\n", camera.midX() - sprite->midX(), deltaPos);
				camera.centerOn(sprite->midX());
			}
		} else if (gameState == FW && deltaPos >= 0)
		{
			//if (sprite->position.x - camera.xpos > deltaPos * 2) {
				//printf("3\n");
				//camera.xpos = min(3000 * deltaTime + camera.xpos,
						//sprite->position.x);
			//} else if (sprite->position.x - camera.xpos > 0) {
				//printf("4\n");
				//camera.xpos = sprite->position.x;
			//}

			//printf("sprite->midX(): %f, camera.midX(): %f\n",
					//sprite->midX(), camera.midX());
			if (sprite->midX() - camera.midX() > deltaPos * 2.0f) {
				printf("3\n");
				//camera.xpos = min(3000 * deltaTime + camera.xpos,
						//sprite->position.x);
				camera.centerOn(min(3000 * deltaTime + camera.midX(),
							sprite->midX()));
			} else if (sprite->midX() - camera.midX() > 0) {
				printf("4\n");
				camera.centerOn(sprite->midX());
			}
		}

		// Update Game State
		bool rightBarrier = sprite->position.x + sprite->width >=
			(camera.xpos + 4.0f * (screenWidth / 5.0f));
		bool leftBarrier = sprite->position.x <=
			(camera.xpos + (screenWidth / 5.0f));
		if (gameState == BW && rightBarrier)
		{
			printf("Now going forward!\n");
			printf("sprite->position.x + sprite->width: %f, (camera.xpos + 4.0f * (screenWidth / 5.0f)): %f\n", sprite->position.x + sprite->position.x, (camera.xpos + 4.0f * (screenWidth / 5.0f)));
			gameState = FW;
		} else if (gameState == FW && leftBarrier)
		{
			printf("Now going backward!\n");
			printf("sprite->position.x: %f, (camera.xpos + (screenWidth / 5.0f)): %f\n", sprite->position.x, (camera.xpos + (screenWidth / 5.0f)));
			gameState = BW;
		}

		sprite->draw(gameState);
		for (int i = 0; i < 5; i++)
		{
			clouds[i]->draw();
		}
		//cloud->draw();

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
	camera.setSize(width, height);
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
		deltaPos = -1000.0f * deltaTime;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		deltaPos = 1000.0f * deltaTime;
	else
		deltaPos = 0.0f;
	sprite->position.x += deltaPos;
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
