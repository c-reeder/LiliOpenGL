#include "Shader.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	string vertexCode;	
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;


	vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		stringstream vShaderStream;

		vShaderStream << vShaderFile.rdbuf();

		vShaderFile.close();

		vertexCode = vShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cerr << "Failed to Read Vertex Shader: " << vertexPath << endl;
	}

	try
	{
		fShaderFile.open(fragmentPath);
		stringstream fShaderStream;

		fShaderStream << fShaderFile.rdbuf();

		fShaderFile.close();

		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cerr << "Failed to Read Fragment Shader: " << fragmentPath << endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "Failed to compile Vertex Shader!\n"
			<< infoLog << endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "Failed to compile Fragment Shader!\n"
			<< infoLog << endl;
	}
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "Failed to link Shader Program!\n"
			<< infoLog << endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::set4f(const std::string &name,
		float x, float y, float z, float w)
{

	glUniform4f(glGetUniformLocation(ID, name.c_str()), x,y,z,w);
}

void Shader::set1i(const std::string &name,
		int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set1f(const std::string &name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set3f(const std::string &name, 
		glm::vec3 vector)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), vector.x, vector.y, vector.z);
}

void Shader::setMatrix4fv(const std::string &name, glm::mat4 matrix)
{
	unsigned int matLoc = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}


