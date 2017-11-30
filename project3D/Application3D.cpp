#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include "../bootstrap/gl_core_4_4.h"
#include "../dependencies/glfw/include/GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "../bootstrap/GameObject.h"
#include "../bootstrap/Texture.h"
#include "../bootstrap/Shader.h"
#include "../bootstrap/Light.h"
#include "../bootstrap/Scene.h"
#include <imgui.h>
#include "../bootstrap/FrameBuffer.h"
using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}
Shader* GiveShader(char* vert, char* frag)
{
	Shader* shader = new Shader();
	shader->m_programID = Shader::CompileShaders(vert, frag);
	return shader;
}
bool Application3D::startup() {
	setBackgroundColour(.5f, .5f, .5f);
	Gizmos::create(10000, 10000, 10000, 10000);
	//loading models
	Model* model = new Model();
	model->LoadFromFile("gun.fbx");
	model->aabb.Fit({ vec3(-30, -5,-11),vec3(20,5,15) });
	Model* animModel = new Model();
	animModel->LoadFromFile("EnemyTank/EnemyTank.fbx");
	animModel->aabb.Fit({ vec3(-1000,0,-900),vec3(1000, 2500 ,800) });
	Model* building = new Model();
	building->MakePPQuad(1000, 1000);
	building->aabb.Fit({ vec3(-1, -1,-1),vec3(1,1,1) });
	//set lights
	m_scene.dirLight = new DirectionalLight(glm::vec3(-.2f, -1, -.3f), glm::vec3(.9f, .9f, .9f));
	m_scene.camera = new Camera(vec3(-10, 80, 70), glm::pi<float>() * 0.25f, -35, -90, (float)getWindowHeight(), (float)getWindowWidth());
	m_scene.pointLights.push_back(new PointLight(glm::vec3(-20, 10, 10), 40, glm::vec3(1, 0, 0)));
	m_scene.pointLights.push_back(new PointLight(glm::vec3(0, 15, 10), 40, glm::vec3(0, 1, 0)));
	m_scene.pointLights.push_back(new PointLight(glm::vec3(0, 70, 10), 40, glm::vec3(0, 0, 1)));
	//setup framebuffers and quads
	DefBuffer = new FrameBuffer(getWindowWidth(), getWindowHeight());
	frameBuffer = new FrameBuffer(getWindowWidth(), getWindowHeight());
	secBuffer = new FrameBuffer(getWindowWidth(), getWindowHeight());
	thirdBuffer = new FrameBuffer(getWindowWidth(), getWindowHeight());
	Model* PPQ;
	PPQ = new Model();
	PPQ->MakePPQuad(getWindowHeight(), getWindowWidth());
	frameBuffer->quad = PPQ;
	DefBuffer->quad = PPQ;
	secBuffer->quad = PPQ;
	thirdBuffer->quad = PPQ;

	//setup shaders for post
	shader.m_programID = Shader::CompileShaders("shaders/vertexShaderAnim.vert", "shaders/Simple.frag");
	superSimpleShader.m_programID = Shader::CompileShaders("shaders/vertexShader.vert", "shaders/Simple.frag");
	ppTechniques.push_back(GiveShader("shaders/PPQuadVertex.vert", "shaders/PPEdgeDetection.frag"));
	ppTechniques.push_back(GiveShader("shaders/DoubleBuffer.vert", "shaders/DoubleBuffer.frag"));
	//make a gameobject for static model

	//make a gameobject for animated model
	GameObject* anim = new GameObject(*animModel, Transform(vec3(-9,10,0), vec3(0, 0, 0), vec3(.01f)));
	anim->shader = GiveShader("shaders/vertexShaderAnim.vert", "shaders/fragmentShader.frag");
	anim->addTexture("EnemyTank/EnemyTank_D.tga", "diffuseMap", aie::Texture::TextureType::DIFFUSE);
	anim->addTexture("EnemyTank/EnemyTank_N.tga", "normMap", aie::Texture::TextureType::NORMAL);
	anim->addTexture("EnemyTank/EnemyTank_S.tga", "specMap", aie::Texture::TextureType::SPECULAR);
	anim->addTexture("EnemyTank/EnemyTank_S.tga", "glossMap", aie::Texture::TextureType::GLOW);
	m_scene.gameObjects.push_back(anim);

	GameObject* Gun = new GameObject(*model, Transform(vec3(0), vec3(0, 0, 0), vec3(.7f)));
	Gun->shader = GiveShader("shaders/vertexShader.vert", "shaders/fragmentShader.frag");
	Gun->addTexture("textures/gun_d.tga", "diffuseMap", aie::Texture::TextureType::DIFFUSE);
	Gun->addTexture("textures/gun_N_1.tga", "normMap", aie::Texture::TextureType::NORMAL);
	Gun->addTexture("textures/gun_S.tga", "specMap", aie::Texture::TextureType::SPECULAR);
	Gun->addTexture("textures/gun_A.tga", "glossMap", aie::Texture::TextureType::GLOW);

	GameObject* Building = new GameObject(*building, Transform(vec3(0,-5,0), vec3(-90, 0, 0), vec3(200,200,1)));
	Building->shader = GiveShader("shaders/simpleDiffuse.vert", "shaders/simpleDiffuse.frag");
	Building->addTexture("textures/Grass.jpg", "diffuseMap", aie::Texture::TextureType::DIFFUSE);
	
	m_scene.gameObjects.push_back(Gun);
	m_scene.gameObjects.push_back(Building);
	m_scene.gameObjects.push_back(new GameObject(*Gun, Transform(vec3(15,35,0), vec3(0,0,90), vec3(1))));
	m_scene.gameObjects.push_back(new GameObject(*Gun, Transform(vec3(-20, 50, 10), vec3(0, 180, 0), vec3(.8f))));
	m_scene.gameObjects.push_back(new GameObject(*Gun, Transform(vec3(-35, 15, 0), vec3(0, 0, -90), vec3(1))));

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
}

void Application3D::update(float deltaTime)
{

	Gizmos::clear();
	aie::Input* input = aie::Input::getInstance();
	m_scene.camera->Update();
	for each (auto light in m_scene.pointLights)
	{
		Gizmos::addSphere(light->position, 1, 3, 3, vec4(1, 1, 0, 1));
	}
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	ImGui::Begin("Lights");
	static int num = 0;
	ImGui::InputInt("Light", &num);
	(num < 0) ? num = m_scene.pointLights.size() - 1 : num = (num % (m_scene.pointLights.size()));
	ImGui::DragFloat3("Point Light Pos", &m_scene.pointLights[num]->position.x);
	ImGui::ColorEdit3("Point Light Color", &m_scene.pointLights[num]->colour.x);
	ImGui::DragFloat("Radius", &m_scene.pointLights[num]->radius, .1f, 0, 10000);
	if (ImGui::Button("Duplicate"))
	{
		m_scene.pointLights.push_back(new PointLight(m_scene.pointLights[num]->position, m_scene.pointLights[num]->radius, m_scene.pointLights[num]->colour));
	}
	ImGui::End();

	ImGui::Begin("GameObjects");
	static int index = 0;
	static int numObjects = 0;
	ImGui::InputInt("object", &index);
	(index < 0) ? index = m_scene.gameObjects.size() - 1 : index = (index % (m_scene.gameObjects.size()));
	ImGui::DragFloat3("Object Position", &m_scene.gameObjects[index]->m_transform.position.x);
	ImGui::DragFloat3("Object Rotation", &m_scene.gameObjects[index]->m_transform.rotation.x, 1, -180, 180);
	ImGui::DragFloat3("Object Scale", &m_scene.gameObjects[index]->m_transform.scale.x, .1f, 0, 100);
	if (m_scene.gameObjects[index]->model->isAnimated())
	{
		ImGui::DragFloat("AnimTime", &m_scene.gameObjects[index]->time, .05f);
	}
	ImGui::Checkbox("Active", &m_scene.gameObjects[index]->active);
	if (ImGui::Button("Duplicate"))
	{
		for (int i = 0; i < numObjects; i++)
		{
			GameObject* GO = new GameObject(*m_scene.gameObjects[index]);
			m_scene.gameObjects.push_back(GO);
		}
	}
	ImGui::SameLine();
	ImGui::DragInt("num", &numObjects, 1, 1, 1000);
	ImGui::End();

	ImGui::Begin("Bounding Boxes");

	ImGui::SliderFloat3("Box min", &m_scene.gameObjects[index]->model->aabb.min.x, -1000, 1000);
	ImGui::SliderFloat3("Box max", &m_scene.gameObjects[index]->model->aabb.max.x, -1000, 1000);
	ImGui::End();

	ImGui::Begin("Fps");
	ImGui::Text(std::to_string(m_fps).c_str());
	ImGui::End();

	ImGui::Begin("Post Processing");

	static bool PPREADY;
	static int ppTechnique = 0;
	ImGui::Checkbox("Post Process", &PPREADY);

	if (PPREADY)
	{
		int techniques = 3;
		ImGui::InputInt("Technique", &ppTechnique);
		(ppTechnique < 0) ? ppTechnique = techniques - 1 : ppTechnique = (ppTechnique % (techniques));
		ImGui::Begin("Post Processing");
		static float num = 5;
		ImGui::InputFloat("Post Processing number", &num, .1f, 1);
		ImGui::End();
		//post process edge detection simple
		if (ppTechnique == 0)
		{
			frameBuffer->RenderScene(m_scene);
			frameBuffer->Draw(ppTechniques[0]->m_programID, num);
		}
		//post process hard edges
		else if (ppTechnique == 1)
		{
			frameBuffer->RenderSceneWithAnim(m_scene, shader.m_programID, superSimpleShader.m_programID, num);
			secBuffer->RenderScene(m_scene, ppTechniques[0]->m_programID, num, frameBuffer);
			DefBuffer->RenderScene(m_scene);
			DefBuffer->Draw(ppTechniques[1]->m_programID, secBuffer);
		}
		//post process combined
		else if (ppTechnique == 2)
		{
			ImGui::Begin("Post Processing");
			static float num2 = 5;
			ImGui::InputFloat("Post Processing sec number", &num2, .1f, 1);
			ImGui::End();
			frameBuffer->RenderSceneWithAnim(m_scene, shader.m_programID, superSimpleShader.m_programID, num2);
			secBuffer->RenderScene(m_scene, ppTechniques[0]->m_programID, num2, frameBuffer);
			DefBuffer->RenderScene(m_scene);
			thirdBuffer->RenderScene(m_scene, ppTechniques[0]->m_programID, num, DefBuffer);
			thirdBuffer->Draw(ppTechniques[1]->m_programID, secBuffer);
		}
	}
	else
		m_scene.Draw();

	Gizmos::draw(m_scene.camera->GetProjViewMat());
	ImGui::End();
}

