/**
 * A class which represents a single sprite (character) within the game.
 */
#ifndef SPRITE_H
#define SPRITE_H
#include "Shader.hpp"

class Sprite
{
	public:
		Sprite(int textureUnit);
		void draw(glm::mat4 projection);

		float height;
		float width;
		glm::vec2 position;

	private:
		Shader shader;
		unsigned int VAO;
		int textureUnit;
		static float vertices[];
		int textures[4];
		float lastTextureSwitch;
		int lastTextureIdx;

};
#endif
