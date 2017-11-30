#pragma once
#include "../bootstrap/gl_core_4_4.h"
#include "../dependencies/glfw/include/GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class PointLight
{
public:
	PointLight(glm::vec3 pos, float rad, glm::vec3 color = glm::vec3(1, 1, 1)) : position(pos), radius(rad), colour(color) {};
	~PointLight();
	void SetUniforms(int lightNum, unsigned int program_ID);
	glm::vec3 position;
	float radius;
	glm::vec3 colour;
};
class DirectionalLight
{
public:
	DirectionalLight(glm::vec3 dir, glm::vec3 colour) : direction(dir), colour(colour) {};
	void SetUniforms(unsigned int program_ID);
	glm::vec3 direction;
	glm::vec3 colour;
};
