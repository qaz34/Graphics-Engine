#pragma once
#include "GameObject.h"
#include "Light.h"
#include "Camera.h"
#include <vector>
class Scene
{
public:
	Scene();
	~Scene();
	void Draw();
	//draw call for post processing to use with animations
	void Draw(unsigned int programID, unsigned int animProgramID = 0);
	std::vector<PointLight*> pointLights;
	DirectionalLight* dirLight;
	std::vector<GameObject*> gameObjects;
	Camera* camera;
};

