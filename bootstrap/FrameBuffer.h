#pragma once
#include "Scene.h"

class FrameBuffer
{
public:
	FrameBuffer(int w, int h) : width(w), height(h) { SetUp();  };
	~FrameBuffer();
	void SetUp();
	//basic render
	void RenderScene(Scene& scene);
	//render with specific shader for scene without animated characters
	void RenderScene(Scene & scene, unsigned int programID, float num, FrameBuffer *targetBuffer = nullptr);
	//render with specific shaders for scene with animated charaters
	void RenderSceneWithAnim(Scene & scene, unsigned int animProgramID, unsigned int programID, float num, FrameBuffer *targetBuffer = nullptr);
	//draw scene with pp shader and a changeable number
	void Draw(unsigned int shader, float num);
	//draw scene with 2 pp effects
	void Draw(unsigned int shader, FrameBuffer* frameBuffer);
	Model* quad;
	int width, height;
	unsigned int m_fbo, m_fboTexture, m_fboDepth;
};

