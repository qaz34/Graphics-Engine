#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "../bootstrap/Camera.h"
#include "../bootstrap/Shader.h"
#include <vector>
#include "../dependencies/tinyobjloader-master/tiny_obj_loader.h"
#include "../bootstrap/Model.h"
#include "../bootstrap/GameObject.h"
#include "../bootstrap/Light.h"
#include "../bootstrap/FrameBuffer.h"
#include "../bootstrap/Scene.h"
class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();


protected:
	bool toggle;
	Shader shader, animShader, superSimpleShader;
	std::vector<Shader*> ppTechniques;
	Scene m_scene;
	FrameBuffer* frameBuffer;
	FrameBuffer* secBuffer;
	FrameBuffer* thirdBuffer;
	FrameBuffer* DefBuffer;
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
};