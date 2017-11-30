#include "GameObject.h"
#include "gl_core_4_4.h"
#include "Gizmos.h"
#include "../glfw/include/GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <imgui.h>
#include <string>
#include "Texture.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "Scene.h"
GameObject::GameObject()
{
}

GameObject::GameObject(GameObject & go, Transform transform) : m_transform(transform)
{
	transform = go.m_transform;
	model = go.model;
	shader = go.shader;
	textures = go.textures;
}

GameObject::GameObject(Model& _model, Transform transform) : m_transform(transform)
{
	model = &_model;
	textures.reserve(10);
}

void GameObject::addTexture(char* filePath, std::string name, aie::Texture::TextureType type)
{
	textures.push_back(new aie::Texture(filePath, type, name));
}


GameObject::~GameObject()
{
}

glm::mat4 GameObject::getTransformMat()
{
	glm::mat4 tempTrans;

	tempTrans = glm::translate(m_transform.position);
	tempTrans *= glm::rotate(glm::radians(m_transform.rotation.y), vec3(0, 1, 0));
	tempTrans *= glm::rotate(glm::radians(m_transform.rotation.z), vec3(0, 0, 1));
	tempTrans *= glm::rotate(glm::radians(m_transform.rotation.x), vec3(1, 0, 0));
	tempTrans *= glm::scale(m_transform.scale);

	if (parent != nullptr)
	{
		glm::mat4 boneMat = parent->model->fbxFile->getSkeletonByIndex(0)->m_nodes[bone]->m_globalTransform;
		tempTrans = parent->getTransformMat() * boneMat * tempTrans;
	}

	return tempTrans;
}

void GameObject::update()
{
	if (model->isAnimated())
	{
		model->updateAnim(time);
	}
}
void GameObject::useProgram(unsigned int programID)
{
	glUseProgram(programID);
}
void GameObject::setMatrix4(unsigned int programID, char * name, glm::mat4& value)
{
	unsigned int handle = glGetUniformLocation(programID, name);
	glUniformMatrix4fv(handle, 1, false, glm::value_ptr(value));
}
void GameObject::Setup(Scene& scene)
{
	glm::mat4 trans = getTransformMat();
	glm::mat4 mvp = scene.camera->GetProjViewMat() * trans;
	useProgram(shader->m_programID);
	shader->BindTextures(textures);
	setMatrix4(shader->m_programID, "MVP", mvp);
	setMatrix4(shader->m_programID, "M", trans);
	setVector4(shader->m_programID, "cameraPosition", glm::vec4(scene.camera->m_position, 0));
	int i = 0;
	for (auto light : scene.pointLights)
	{
		light->SetUniforms(i, shader->m_programID);
		i++;
	}
	setInt(shader->m_programID, "lightsNum", i);
	scene.dirLight->SetUniforms(shader->m_programID);
}
void GameObject::setVector4(unsigned int programID, char * name, glm::vec4& value)
{
	unsigned int handle = glGetUniformLocation(programID, name);
	glUniform4f(handle, value.x, value.y, value.z, value.w);
}
void GameObject::draw(Scene& scene)
{
	if (active)
	{
		update();
		Setup(scene);
		model->Draw(shader->m_programID);

		aie::Gizmos::addAABB(model->aabb.center(), model->aabb.extents(), glm::vec4(1), &getTransformMat());
	}
}
void GameObject::updateTransformFromParent()
{

}
void GameObject::draw(Scene & scene, unsigned int programID)
{
	if (active)
	{
		model->Draw(programID);
	}
}
void GameObject::setInt(unsigned int programID, char * name, int value)
{
	unsigned int handle = glGetUniformLocation(programID, name);
	glUniform1i(handle, value);
}