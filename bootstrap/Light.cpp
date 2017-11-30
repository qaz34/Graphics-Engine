#include "Light.h"
#include "../bootstrap/gl_core_4_4.h"
#include "../dependencies/glfw/include/GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

PointLight::~PointLight()
{
}

void PointLight::SetUniforms(int lightNum, unsigned int program_ID)
{
	std::string strRad, strPos, strcolor;
	strRad = "lights[" + std::to_string(lightNum) + "].radius";
	strPos = "lights[" + std::to_string(lightNum) + "].position";
	strcolor = "lights[" + std::to_string(lightNum) + "].colour";
	glUniform1f(glGetUniformLocation(program_ID, strRad.c_str()), radius);
	glUniform3f(glGetUniformLocation(program_ID, strPos.c_str()), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(program_ID, strcolor.c_str()), colour.x, colour.y, colour.z);
}

void DirectionalLight::SetUniforms(unsigned int program_ID)
{
	glUniform3f(glGetUniformLocation(program_ID, "dirLight.dir"), direction.x, direction.y, direction.z);
	glUniform3f(glGetUniformLocation(program_ID, "dirLight.colour"), colour.x, colour.y, colour.z);
}
