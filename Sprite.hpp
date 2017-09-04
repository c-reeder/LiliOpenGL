/**
 * A class which represents a single sprite (character) within the game.
 */
#ifndef SPRITE_H
#define SPRITE_H
#include "Shader.hpp"
#include "Game.hpp"
#include "Camera_2D.hpp"

class Sprite
{
	public:
		Sprite(Camera_2D* camera, int textureUnit);
		void draw(Game_State state);

		float height;
		float width;
		glm::vec2 position;

	private:
		Camera_2D* camera;
		Shader shader;
		unsigned int VAO;
		int textureUnit;
		static float vertices[];
		int textures[4];
		float lastTextureSwitch;
		int lastTextureIdx;

};
#endif
