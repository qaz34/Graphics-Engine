#include "Scene.h"
#include <algorithm>
#include <imgui.h>

Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::Draw()
{
	static bool cameraCull = false;
	ImGui::Begin("Camera");
	ImGui::Checkbox("CameraCulling", &cameraCull);
	ImGui::End();
	if (cameraCull)
	{		
		float edge = 1.0f;
		glm::mat4 projView = camera->GetProjViewMat();
		for each (auto object in gameObjects)
		{
			if (object->active)
			{				
				glm::vec4 corner;
				glm::vec4 screenPos[8];
				glm::vec3 screenMin(FLT_MAX, FLT_MAX, FLT_MAX);
				glm::vec3 screenMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);
				for (int i = 0; i < 8; i++)
				{
					corner.x = (i & 1) ? object->model->aabb.min.x : object->model->aabb.max.x;
					corner.y = (i & 2) ? object->model->aabb.min.y : object->model->aabb.max.y;
					corner.z = (i & 4) ? object->model->aabb.min.z : object->model->aabb.max.z;
					corner.w = 1;
					screenPos[i] = projView * object->getTransformMat() * corner;
					//do the perspective divide
					screenPos[i].x /= screenPos[i].w;
					screenPos[i].y /= screenPos[i].w;
					screenPos[i].z /= screenPos[i].w;
					screenMin.x = std::min(screenMin.x, screenPos[i].x);
					screenMin.y = std::min(screenMin.y, screenPos[i].y);
					screenMin.z = std::min(screenMin.z, screenPos[i].z);
					screenMax.x = std::max(screenMax.x, screenPos[i].x);
					screenMax.y = std::max(screenMax.y, screenPos[i].y);
					screenMax.z = std::max(screenMax.z, screenPos[i].z);
				}

				if (screenMax.x < -edge || screenMin.x > edge || screenMax.y < -edge || screenMin.y > edge || screenMax.z < -1 || screenMin.z > 1) {}
				else
				{
					object->draw(*this);
				}
			}
		}
	}
	else
		for each (auto object in gameObjects)
		{
			object->draw(*this);
		}
}

void Scene::Draw(unsigned int programID, unsigned int animProgramID)
{

	for each (auto object in gameObjects)
	{

		if (object->model->isAnimated() && animProgramID != 0)
		{
			glUseProgram(animProgramID);
			glUniformMatrix4fv(glGetUniformLocation(animProgramID, "MVP"), 1, false, glm::value_ptr(camera->GetProjViewMat() *object->getTransformMat()));
			object->draw(*this, animProgramID);
		}
		else
		{
			glUseProgram(programID);
			glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, false, glm::value_ptr(camera->GetProjViewMat() *object->getTransformMat()));
			object->draw(*this, programID);
		}

	}
}
