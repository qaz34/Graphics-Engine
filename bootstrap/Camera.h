#pragma once
#include <glm/glm.hpp>

using glm::vec3;
using glm::mat4;
class Camera
{
protected:

	float m_fov;
	float m_vertAngle;
	float m_hozAngle;
	const float deg2rad = 3.14159f / 180.f;
	int m_lastMouseX;
	int m_lastMouseY;
	float w, h;
public:
	Camera();
	vec3 m_position;
	Camera(vec3 pos, float fov, float lat, float lng, float height, float width);
	~Camera();
	mat4 GetProjectionMatrix();
	mat4 GetViewMatrix();
	mat4 GetProjViewMat();
	void GetFrustumPlanes(glm::vec4* planes);
	void Update();

};

