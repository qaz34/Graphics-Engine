#include "FrameBuffer.h"
#include <imgui.h>
#include "gl_core_4_4.h"
#include "../glfw/include/GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
FrameBuffer::~FrameBuffer()
{

}

void FrameBuffer::SetUp()
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	// generate a new texture, and set it as the current texture
	glGenTextures(1, &m_fboTexture);
	glBindTexture(GL_TEXTURE_2D, m_fboTexture);
	// allocate for width x height, with RGB 8 bytes each on the GPU
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);
	// set some filtering parameters on the texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// attach this texture and its data to the current framebuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_fboTexture, 0);
	// create a new depth buffer and set it as the current one
	glGenRenderbuffers(1, &m_fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth);
	// allocate memory on the GPU for the depth buffer
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	// attach this depth buffer to the current frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fboDepth);
	// test everything's set up correctly
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer Error!\n");
	// we're done! detach the frame buffer, so the current one is once again the default screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::RenderScene(Scene & scene)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, width, height);
	glClearColor(0.5f, .5f, .5f, 1); // could pass this in as argument, or ignore altogether
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw the actual scene here
	scene.Draw();
	// restore normal frame buffer after
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClearColor(0.5f, .5f, .5f, 1); // could pass this in as argument, or ignore altogether
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::RenderScene(Scene & scene, unsigned int programID, float num, FrameBuffer *targetBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, width, height);
	glClearColor(0.5f, .5f, .5f, 1); // could pass this in as argument, or ignore altogether
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw the actual scene here
	if (targetBuffer == nullptr)
		scene.Draw(programID);
	else
		targetBuffer->Draw(programID, num);
	// restore normal frame buffer after
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClearColor(0.5f, .5f, .5f, 1); // could pass this in as argument, or ignore altogether
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::RenderSceneWithAnim(Scene & scene, unsigned int animProgramID, unsigned int programID, float num, FrameBuffer * targetBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, width, height);
	glClearColor(0.5f, .5f, .5f, 1); // could pass this in as argument, or ignore altogether
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw the actual scene here
	if (targetBuffer == nullptr)
		scene.Draw(programID, animProgramID);
	else
		targetBuffer->Draw(programID, num);
	// restore normal frame buffer after
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClearColor(0.5f, .5f, .5f, 1); // could pass this in as argument, or ignore altogether
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::Draw(unsigned int shader, float num)
{
	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fboTexture);
	int loc = glGetUniformLocation(shader, "target");
	glUniform1i(loc, 0);
	glUniform1f(glGetUniformLocation(shader, "amount"), num);
	glBindVertexArray(quad->m_glInfo[0].m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void FrameBuffer::Draw(unsigned int shader, FrameBuffer * frameBuffer)
{
	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fboTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, frameBuffer->m_fboTexture);
	glUniform1i(glGetUniformLocation(shader, "target1"), 0);
	glUniform1i(glGetUniformLocation(shader, "target2"), 1);
	glBindVertexArray(quad->m_glInfo[0].m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
