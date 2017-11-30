#include "Camera.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Gizmos.h"
#include  <algorithm>
using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

float clamp(float value, float min, float max)
{
	return  value < min ? min : (value > max ? max : value);
}

Camera::Camera()
{
	m_hozAngle = 0;
	m_vertAngle = -20;
	m_position = vec3(-10, 4, 0);
	m_fov = glm::pi<float>() * 0.25f;
}

Camera::Camera(vec3 pos, float fov, float lat, float lng, float height, float width)
{
	m_position = pos;
	m_fov = fov;
	m_vertAngle = lat;
	m_hozAngle = lng;
	w = width;
	h = height;
}

Camera::~Camera()
{
}

mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(m_fov, w / h, .1f, 1000.f);
}

mat4 Camera::GetViewMatrix()
{
	float thetaR = m_hozAngle * deg2rad;
	float phiR = m_vertAngle * deg2rad;
	vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

mat4 Camera::GetProjViewMat()
{
	return GetProjectionMatrix() * GetViewMatrix();
}

void Camera::GetFrustumPlanes(glm::vec4 * planes)
{
	glm::mat4 transform = GetProjViewMat();
	planes[0] = vec4(transform[0][3] - transform[0][0], transform[1][3] - transform[1][0], transform[2][3] - transform[2][0], transform[3][3] - transform[3][0]);
	// left side
	planes[1] = vec4(transform[0][3] + transform[0][0], transform[1][3] + transform[1][0], transform[2][3] + transform[2][0], transform[3][3] + transform[3][0]);
	// top
	planes[2] = vec4(transform[0][3] - transform[0][1], transform[1][3] - transform[1][1], transform[2][3] - transform[2][1], transform[3][3] - transform[3][1]);
	// bottom
	planes[3] = vec4(transform[0][3] + transform[0][1], transform[1][3] + transform[1][1], transform[2][3] + transform[2][1], transform[3][3] + transform[3][1]);
	// far
	planes[4] = vec4(transform[0][3] - transform[0][2], transform[1][3] - transform[1][2], transform[2][3] - transform[2][2], transform[3][3] - transform[3][2]);
	// near 
	planes[5] = vec4(transform[0][3] + transform[0][2], transform[1][3] + transform[1][2], transform[2][3] + transform[2][2], transform[3][3] + transform[3][2]);
	// plane normalisation, based on length of normal 
	for (int i = 0; i < 6; i++)
	{
		float d = glm::length(vec3(planes[i]));
		planes[i] /= d;
	}
}

void Camera::Update()
{
	aie::Input* input = aie::Input::getInstance();
	float thetaR = m_hozAngle * deg2rad;

	float phiR = m_vertAngle * deg2rad;

	vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	vec3 right(-sin(thetaR), 0, cos(thetaR));
	vec3 up(0, 1, 0);
	float deltaTime = 0.1f;
	if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))
		deltaTime = 1.0f;
	if (input->isKeyDown(aie::INPUT_KEY_X))
		m_position += up * deltaTime;
	if (input->isKeyDown(aie::INPUT_KEY_Z))
		m_position += -up * deltaTime;
	if (input->isKeyDown(aie::INPUT_KEY_A))
		m_position += -right * deltaTime;
	if (input->isKeyDown(aie::INPUT_KEY_D))
		m_position += right * deltaTime;
	if (input->isKeyDown(aie::INPUT_KEY_W))
		m_position += forward * deltaTime;
	if (input->isKeyDown(aie::INPUT_KEY_S))
		m_position += -forward * deltaTime;
	int mx = input->getMouseX();
	int my = input->getMouseY();
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_hozAngle += 0.2f * (mx - m_lastMouseX);
		m_vertAngle += 0.2f * (my - m_lastMouseY);
		m_vertAngle = clamp(m_vertAngle, -70, 70);
	}
	m_lastMouseX = mx;
	m_lastMouseY = my;
}
