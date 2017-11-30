#pragma once
#include "gl_core_4_4.h"
#include "../glfw/include/GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <string>
#include "Model.h"
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"
#include "Light.h"
class Scene;
struct Transform
{
	Transform()
	{
		position = glm::vec3(0, 0, 0);
		rotation = glm::vec3(0, 0, 0);
		scale = glm::vec3(1, 1, 1);
	}
	Transform(glm::vec3 pos, glm::vec3 rota, glm::vec3 _scale)
	{
		position = pos;
		rotation = rota;
		scale = _scale;
	}
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

class GameObject
{
public:

	Model* model;
	GameObject();
	GameObject(GameObject& go, Transform transform = Transform());
	GameObject(Model& _model, Transform transform = Transform());
	void setInt(unsigned int programID, char * name, int value);
	void addTexture(char* filePath, std::string name, aie::Texture::TextureType type);
	~GameObject();
	//return a matrix made out of the transform struct
	glm::mat4 getTransformMat();
	//update any animations
	void update();
	float time = 0;
	Shader* shader;
	std::vector<aie::Texture*> textures;
	//sets lights and transforms
	void Setup(Scene& scene);
	void setVector4(unsigned int programID, char * name, glm::vec4& value);
	void useProgram(unsigned int programID);
	void setMatrix4(unsigned int programID, char * name, glm::mat4& value);
	//basic draw
	void draw(Scene& scene);
	void updateTransformFromParent();
	//draw with no lights and specific shader
	void draw(Scene& scene, unsigned int programID);
	bool active = true;
	int bone = 40;
	GameObject* parent = nullptr;
	Transform  m_transform;
};

