#pragma once
#include "gl_core_4_4.h"
#include "../glfw/include/GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "Texture.h"
#include "Camera.h"
#include "../dependencies/FBXLoader/FBXFile.h"

struct OpenGLInfo
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_faceCount;
};
struct OBJVertex
{
	float x, y, z, w;
	float nx, ny, nz, nw;
	float tx, ty, tz, tw;
	float u, v;
};
class AABB {
public:
	AABB() { Reset(); }
	void Reset();
	void Fit(const std::vector<glm::vec3>& points);
	glm::vec3 center() { return ((min + max) / 2); }
	glm::vec3 extents() { return ((max - min) / 2); }
	glm::vec3 min, max;
};
class Model
{
protected:

	void LoadOBJ(char * fileName);
	void LoadFBX(char * fileName);
public:
	std::vector<OpenGLInfo> m_glInfo;
	Model();
	~Model();
	//helper functions
	void LoadFromFile(char* fileName);
	void setFloat(unsigned int programID, char* name, float value);
	void setMatrix4(unsigned int programID, char* name, glm::mat4 &value);
	void setMatrix4(unsigned int programID, int size, char* name, glm::mat4 &value);
	void setVector4(unsigned int programID, char* name, glm::vec4 value);
	void useProgram(unsigned int programID);
	void bindTexture(int slot, int textureHandle);
	//draw model
	void Draw(unsigned int programID);
	void updateAnim(float& frame);
	bool isAnimated() { return fbxFile && fbxFile->getSkeletonCount() > 0; }
	void MakePPQuad(int h, int w);

	FBXFile* fbxFile = nullptr;
	AABB aabb;
};

