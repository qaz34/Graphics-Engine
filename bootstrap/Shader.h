#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Texture.h"
using glm::vec4;
using glm::vec3;
using glm::mat4;
using std::string;

struct Vertex
{
	vec4 pos;
	vec4 colour;
};
class Shader
{

public:

	unsigned int m_programID;
	Shader();
	~Shader();
	static unsigned int CompileShaders(const char* vsFile, const char* fsFile);
	static bool LoadShaderFromFile(const char* filePath, string& code);
	void BindTexture(aie::Texture* texture);
	void BindTextures(std::vector<aie::Texture*>& textures);
	void UnbindTextures();
};

