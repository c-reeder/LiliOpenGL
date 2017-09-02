#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <glm/glm.hpp>

class Shader
{
	public:
		unsigned int ID;

		Shader(const char* vertexPath, const char* fragmentPath);

		void use();

		// Set individual Values
		void set1i(const std::string &name,
				int value);
		void set1f(const std::string &name, float value);

		// Set Vectors
		void set4f(const std::string &name,
				float x, float y, float z, float w);
		void set3f(const std::string &name, 
				glm::vec3 vector);

		// Set Matrices
		void setMatrix4fv(const std::string &name, glm::mat4 matrix);
};

#endif
