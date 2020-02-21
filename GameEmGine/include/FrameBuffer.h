#pragma once

#include <GL/glew.h>
#include <functional>
#include <string>
#include "Shader.h"

class FrameBuffer
{
public:
	FrameBuffer() = delete;
	FrameBuffer(std::string tag, unsigned numColorAttachments);
	~FrameBuffer();

	void initDepthTexture(unsigned width, unsigned height);
	void resizeDepth(unsigned width, unsigned height);
	void resizeColour(unsigned index, unsigned width, unsigned height, GLint internalFormat, GLint filter, GLint wrap);
	void resizeColour(unsigned index, unsigned width, unsigned height);
	void initColourTexture(unsigned m_index, unsigned width, unsigned height, GLint internalFormat, GLint filter, GLint wrap);
	bool checkFBO();

	// Clears all OpenGL memory
	void unload();

	// Clears all attached textures
	void clear();

	//binds objects to frame buffer/s
	void enable();

	//binds the frame buffer to the default spot
	static void disable();


	///~ Helper Functions ~///
	void setViewport(int x, int y, int width, int height)const;

	void moveColourToBackBuffer(int windowWidth, int windowHeight);

	void moveColourToBuffer(int windowWidth, int windowHeight, GLuint fboID);

	void moveDepthToBackBuffer(int windowWidth, int windowHeight);

	void moveDepthToBuffer(int windowWidth, int windowHeight, GLuint fboID);

	void takeFromBackBufferColour(int windowWidth, int windowHeight);

	void takeFromBackBufferDepth(int windowWidth, int windowHeight);

	GLuint getDepthHandle() const;
	GLuint getColorHandle(unsigned m_index) const;

	void setPostProcess(std::function<void()>, unsigned layer = 0);
	std::function<void()> getPostProcess();

	unsigned getNumColourAttachments();

	GLuint getFrameBufferID();


	static void drawFullScreenQuad();

	std::string getTag();
	unsigned getLayer();

private:
	static void initFullScreenQuad();
	
	static GLuint m_fsQuadVAO_ID, m_fsQuadVBO_ID;

	GLuint
		m_layer = GL_NONE,
		m_fboID = GL_NONE,
		m_depthAttachment = GL_NONE,
		* m_colorAttachments = nullptr;

	GLint m_internalFormat = GL_RGBA8,
		m_filter = GL_LINEAR,
		m_wrap = GL_CLAMP_TO_EDGE;

	unsigned m_width, m_height;

	GLenum* m_buffs = nullptr;

	unsigned int m_numColorAttachments = 0;
	std::string m_tag;
	std::function<void()>m_postProcess;
	Shader* m_shader;
};