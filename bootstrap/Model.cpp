#include "Model.h"
#include "Gizmos.h"
#include "Input.h"
#include "gl_core_4_4.h"
#include "../glfw/include/GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include "../dependencies/tinyobjloader-master/tiny_obj_loader.h"
#include "Texture.h"
#include "Camera.h"
Model::Model()
{

}


Model::~Model()
{
	if (fbxFile != nullptr)
		delete fbxFile;
	m_glInfo.clear();
	m_glInfo.shrink_to_fit();
}
void Model::LoadOBJ(char * fileName)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	bool success = tinyobj::LoadObj(&attribs, &shapes, &materials, &err, fileName);

	m_glInfo.resize(shapes.size());
	// grab each shape 
	int shapeIndex = 0;
	for (auto& shape : shapes)
	{
		// setup OpenGL data 
		glGenVertexArrays(1, &m_glInfo[shapeIndex].m_VAO);
		glGenBuffers(1, &m_glInfo[shapeIndex].m_VBO);
		glBindVertexArray(m_glInfo[shapeIndex].m_VAO);
		m_glInfo[shapeIndex].m_faceCount = shape.mesh.num_face_vertices.size();
		// collect triangle vertices 
		std::vector<OBJVertex> vertices;
		int index = 0;
		for (auto face : shape.mesh.num_face_vertices)
		{
			for (int i = 0; i < 3; ++i)
			{
				tinyobj::index_t idx = shape.mesh.indices[index + i];
				OBJVertex v = { 0 };
				// positions 
				v.x = attribs.vertices[3 * idx.vertex_index + 0];
				v.y = attribs.vertices[3 * idx.vertex_index + 1];
				v.z = attribs.vertices[3 * idx.vertex_index + 2];
				// normals 
				if (attribs.normals.size() > 0)
				{
					v.nx = attribs.normals[3 * idx.normal_index + 0];
					v.ny = attribs.normals[3 * idx.normal_index + 1];
					v.nz = attribs.normals[3 * idx.normal_index + 2];
				} // texture coordinates
				if (attribs.texcoords.size() > 0)
				{
					v.u = attribs.texcoords[2 * idx.texcoord_index + 0];
					v.v = -attribs.texcoords[2 * idx.texcoord_index + 1];
				} vertices.push_back(v);
			}
			index += face;
		}
		// bind vertex data 
		glBindBuffer(GL_ARRAY_BUFFER, m_glInfo[shapeIndex].m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(OBJVertex), vertices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), 0);
		glEnableVertexAttribArray(1);
		//normal data 
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(OBJVertex), (void*)12);
		glEnableVertexAttribArray(2);
		//texture data 
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OBJVertex), (void*)24);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		shapeIndex++;
	}
}
void Model::LoadFBX(char * fileName)
{
	fbxFile = new FBXFile();
	fbxFile->load(fileName);
	m_glInfo.resize(fbxFile->getMeshCount());
	for (unsigned int i = 0; i < fbxFile->getMeshCount(); ++i) {
		FBXMeshNode* mesh = fbxFile->getMeshByIndex(i);
		unsigned int* glData = new unsigned int[3];
		glGenVertexArrays(1, &glData[0]);
		glBindVertexArray(glData[0]);
		glGenBuffers(1, &glData[1]);
		glGenBuffers(1, &glData[2]);
		glBindBuffer(GL_ARRAY_BUFFER, glData[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[2]);
		glBufferData(GL_ARRAY_BUFFER, mesh->m_vertices.size() * sizeof(FBXVertex), mesh->m_vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->m_indices.size() * sizeof(unsigned int), mesh->m_indices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0); // position 
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), 0);
		glEnableVertexAttribArray(1); // normal 
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), ((char*)0) + FBXVertex::NormalOffset);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), ((char*)0) + FBXVertex::TexCoord1Offset);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex), ((char*)0) + FBXVertex::TangentOffset);
		if (isAnimated())
		{
			glEnableVertexAttribArray(4); // weights
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), ((char*)0) + FBXVertex::WeightsOffset);
			glEnableVertexAttribArray(5); // indices
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), ((char*)0) + FBXVertex::IndicesOffset);
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		mesh->m_userData = glData;
		m_glInfo[i].m_VAO = glData[0];
		m_glInfo[i].m_VBO = glData[1];
		m_glInfo[i].m_IBO = glData[2];
		m_glInfo[i].m_faceCount = mesh->m_indices.size();
	}
}
void Model::LoadFromFile(char * fileName)
{
	if (strstr(fileName, ".obj") != NULL)
	{
		LoadOBJ(fileName);
	}
	if (strstr(fileName, ".fbx") != NULL)
	{
		LoadFBX(fileName);
	}
}
void Model::setFloat(unsigned int programID, char * name, float value)
{
	unsigned int handle = glGetUniformLocation(programID, name);
	glUniform1f(handle, value);
}
void Model::setMatrix4(unsigned int programID, char * name, glm::mat4& value)
{
	unsigned int handle = glGetUniformLocation(programID, name);
	glUniformMatrix4fv(handle, 1, false, glm::value_ptr(value));
}
void Model::setMatrix4(unsigned int programID, int size, char * name, glm::mat4& value)
{
	unsigned int handle = glGetUniformLocation(programID, name);
	glUniformMatrix4fv(handle, size, false, glm::value_ptr(value));
}
void Model::setVector4(unsigned int programID, char * name, glm::vec4 value)
{
	unsigned int handle = glGetUniformLocation(programID, name);
	glUniform4f(handle, value.x, value.y, value.z, value.w);
}
void Model::useProgram(unsigned int programID)
{
	glUseProgram(programID);
}
void Model::bindTexture(int slot, int textureHandle)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
}
void Model::Draw(unsigned int programID)
{

	if (isAnimated())
	{
		FBXSkeleton* skel = fbxFile->getSkeletonByIndex(0);
		skel->updateBones();
		setMatrix4(programID, (int)skel->m_boneCount, "bones", *skel->m_bones);
	}
	for (auto& gl : m_glInfo)
	{
		glBindVertexArray(gl.m_VAO);
		if (gl.m_IBO == 0)
			glDrawArrays(GL_TRIANGLES, 0, (gl.m_faceCount != 0) ? gl.m_faceCount * 3 : 6);
		else
			glDrawElements(GL_TRIANGLES, gl.m_faceCount * 3, GL_UNSIGNED_INT, 0);
	}
	
}

void Model::updateAnim(float& frame)
{
	FBXSkeleton* skeleton = fbxFile->getSkeletonByIndex(0);
	FBXAnimation* animation = fbxFile->getAnimationByIndex(0);
	skeleton->evaluate(animation, frame);
	for (unsigned int bone_index = 0; bone_index < skeleton->m_boneCount; ++bone_index)
	{
		skeleton->m_nodes[bone_index]->updateGlobalTransform();
		//bone 40 middle of right hand
	}
}

void Model::MakePPQuad(int h, int w)
{
	m_glInfo.resize(1);
	glm::vec2 halfTexel = 1.0f / glm::vec2(w, h) * 0.5f;
	float vertexData[] = {
		-1, -1, 0, 1, halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y,
		-1, 1, 0, 1, halfTexel.x, 1 - halfTexel.y,
		-1, -1, 0, 1, halfTexel.x, halfTexel.y,
		1, -1, 0, 1, 1 - halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y,
	};
	glGenVertexArrays(1, &m_glInfo[0].m_VAO);
	glBindVertexArray(m_glInfo[0].m_VAO);
	glGenBuffers(1, &m_glInfo[0].m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_glInfo[0].m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // position
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1); // UVs
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void AABB::Reset()
{
	min.x = min.y = min.z = 1e37f;
	max.x = max.y = max.z = -1e37f;
}

void AABB::Fit(const std::vector<glm::vec3>& points)
{
	Reset();
	for (auto& p : points)
	{
		if (p.x < min.x) min.x = p.x;
		if (p.y < min.y) min.y = p.y;
		if (p.z < min.z) min.z = p.z;
		if (p.x > max.x) max.x = p.x;
		if (p.y > max.y) max.y = p.y;
		if (p.z > max.z) max.z = p.z;
	}
}