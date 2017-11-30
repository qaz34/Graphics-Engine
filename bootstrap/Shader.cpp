#include "Shader.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <fstream>
#include <string>
#include <vector>
#include "Camera.h"
#include "Texture.h"
#include <iostream>
using glm::vec4;
using glm::vec3;
Shader::Shader()
{
}


Shader::~Shader()
{
	return;
}
bool Shader::LoadShaderFromFile(const char* filePath, std::string& code)
{
	code = "";

	// Read the Vertex Shader code from the file
	std::ifstream VertexShaderStream(filePath, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string line;
		while (getline(VertexShaderStream, line))
			code += line + "\n";
		VertexShaderStream.close();
		return true;
	}
	else {
		printf("Cannot open shader file %s.\n", filePath);
		return false;
	}
}

void Shader::BindTexture(aie::Texture* texture)
{
	glActiveTexture(GL_TEXTURE0 + (int)texture->getFileType());
	glBindTexture(GL_TEXTURE_2D, texture->getHandle());

	unsigned int handle = glGetUniformLocation(m_programID, texture->getName().c_str());
	glUniform1i(handle, (int)texture->getFileType());
}

void Shader::BindTextures(std::vector<aie::Texture*>& textures)
{
	for (aie::Texture* tex : textures)
	{
		BindTexture(tex);
	}
}


void Shader::UnbindTextures()
{
	for (int i = 0; i < aie::Texture::TextureType::ENUM_SIZE; i++)
	{
		std::string charar = aie::Texture::TextureName::GetName(i);
		unsigned int handle = glGetUniformLocation(m_programID, charar.c_str());
		glUniform1i(handle, -1);
	}
}

GLuint Shader::CompileShaders(const char* vsFile, const char* fsFile)
{
	std::string vsCode, fsCode;
	if (!(LoadShaderFromFile(vsFile, vsCode) && LoadShaderFromFile(fsFile, fsCode)))
		printf("failed to load shaders");
	const char* vsSource = vsCode.c_str();
	const char* fsSource = fsCode.c_str();

	GLuint programID;
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	return programID;
}
