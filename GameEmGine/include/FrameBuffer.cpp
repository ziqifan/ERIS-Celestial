#include "FrameBuffer.h"
GLuint FrameBuffer::m_fsQuadVAO_ID, FrameBuffer::m_fsQuadVBO_ID;

FrameBuffer::FrameBuffer(std::string tag, unsigned numColorAttachments)
{
	m_tag = tag;
	glGenFramebuffers(1, &m_fboID);
	m_numColorAttachments = numColorAttachments;

	m_colorAttachments = new GLuint[m_numColorAttachments];
	memset(m_colorAttachments, 0, sizeof(GLuint)* numColorAttachments);
	m_depthAttachment = 0;

	//Buffs is required as a parameter for glDrawBuffers()
	m_buffs = new GLenum[m_numColorAttachments];
	for(unsigned i = 0; i < m_numColorAttachments; i++)
	{
		m_buffs[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	initFullScreenQuad();
}

FrameBuffer::~FrameBuffer()
{
	unload();
}

void FrameBuffer::initDepthTexture(unsigned width, unsigned height)
{
	if(!m_depthAttachment)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);

		//create depth texture
		glGenTextures(1, &m_depthAttachment);

		resizeDepth(width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}
}

void FrameBuffer::initColourTexture(unsigned index, unsigned width, unsigned height, GLint internalFormat, GLint filter, GLint wrap)
{
	if(!m_colorAttachments[index])
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);

		//create colour texture
		glGenTextures(1, &m_colorAttachments[index]);

		resizeColour(index, width, height, internalFormat, filter, wrap);
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}
}

void FrameBuffer::resizeDepth(unsigned width, unsigned height)
{
	if(m_depthAttachment)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
		glDeleteTextures(1, &m_depthAttachment);
		glGenTextures(1, &m_depthAttachment);

		m_width = width;
		m_height = height;
		glBindTexture(GL_TEXTURE_2D, m_depthAttachment);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//Bind texture to the fbo
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void FrameBuffer::resizeColour(unsigned index, unsigned width, unsigned height, GLint internalFormat, GLint filter, GLint wrap)
{
	if(m_colorAttachments[index])
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
		glDeleteTextures(1, &m_colorAttachments[index]);
		glGenTextures(1, &m_colorAttachments[index]);

		m_internalFormat = internalFormat;
		m_filter = filter;
		m_wrap = wrap;

		glBindTexture(GL_TEXTURE_2D, m_colorAttachments[index]);
		glTexStorage2D(GL_TEXTURE_2D, 1, m_internalFormat, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap);

		//Bind texture to the fbo
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, m_colorAttachments[index], 0);

		glBindTexture(GL_TEXTURE_2D,0);
	}
}

void FrameBuffer::resizeColour(unsigned index, unsigned width, unsigned height)
{
	if(m_colorAttachments[index])
	{		
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
		glBindTexture(GL_TEXTURE_2D, m_colorAttachments[index]);
		glTexStorage2D(GL_TEXTURE_2D, 1, m_internalFormat, width, height);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap);

		//Bind texture to the fbo
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, m_colorAttachments[index], 0);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

bool FrameBuffer::checkFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		unload();
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

	return true;
}

// Clears all OpenGL memory
void FrameBuffer::unload()
{
	if(m_buffs != nullptr)
	{
		delete[] m_buffs;
		m_buffs = nullptr;
	}

	if(m_colorAttachments != nullptr)
	{
		for(unsigned i = 0; i < m_numColorAttachments; i++)
		{
			glDeleteTextures(1, &m_colorAttachments[i]);
		}

		delete[] m_colorAttachments;
		m_colorAttachments = nullptr;
	}

	if(m_depthAttachment != GL_NONE)
	{
		glDeleteTextures(1, &m_depthAttachment);
		m_depthAttachment = GL_NONE;
	}

	m_numColorAttachments = 0;
}

// Clears all attached textures
void FrameBuffer::clear()
{
	GLbitfield temp = 0;

	if(m_depthAttachment != GL_NONE)
	{
		temp = temp | GL_DEPTH_BUFFER_BIT;
	}

	if(m_colorAttachments != nullptr)
	{
		temp = temp | GL_COLOR_BUFFER_BIT;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
	glClear(temp);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::enable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
	glDrawBuffers(m_numColorAttachments, m_buffs);
}

void FrameBuffer::disable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::setViewport(int x, int y, int width, int height) const
{
	glViewport(x, y, width, height);
}

void FrameBuffer::moveColourToBackBuffer(int windowWidth, int windowHeight)
{
	moveColourToBuffer(windowWidth, windowHeight, GL_NONE);
}

void FrameBuffer::moveColourToBuffer(int windowWidth, int windowHeight, GLuint fboID)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboID);

	glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::moveDepthToBackBuffer(int windowWidth, int windowHeight)
{
	moveDepthToBuffer(windowWidth, windowHeight, GL_NONE);
}

void FrameBuffer::moveDepthToBuffer(int windowWidth, int windowHeight, GLuint fboID)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboID);

	glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::takeFromBackBufferColour(int windowWidth, int windowHeight)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GL_NONE);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fboID);

	glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void FrameBuffer::takeFromBackBufferDepth(int windowWidth, int windowHeight)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GL_NONE);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fboID);

	glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

GLuint FrameBuffer::getDepthHandle() const
{
	return m_depthAttachment;
}
GLuint FrameBuffer::getColorHandle(unsigned m_index) const
{
	return m_colorAttachments[m_index];
}

void FrameBuffer::setPostProcess(std::function<void()>post, unsigned layer)
{
	m_postProcess = post;
	m_layer = layer;
}

std::function<void()> FrameBuffer::getPostProcess()
{
	return m_postProcess;
}

unsigned FrameBuffer::getNumColourAttachments()
{
	return m_numColorAttachments;
}

GLuint FrameBuffer::getFrameBufferID()
{
	return m_fboID;
}

void FrameBuffer::initFullScreenQuad()
{
	float vboData[] =
	{
		-1.0f,-1.0f,0.0f,
		 1.0f,-1.0f,0.0f,
		-1.0f, 1.0f,0.0f,

		 1.0f, 1.0f,0.0f,
		-1.0f, 1.0f,0.0f,
		 1.0f,-1.0f,0.0f,

		0.0f,0.0f,
		1.0f,0.0f,
		0.0f,1.0f,

		1.0f,1.0f,
		0.0f,1.0f,
		1.0f,0.0f
	};

	if(!m_fsQuadVAO_ID)
		glGenVertexArrays(1, &m_fsQuadVAO_ID);

	glBindVertexArray(m_fsQuadVAO_ID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	if(!m_fsQuadVBO_ID)
		glGenBuffers(1, &m_fsQuadVBO_ID);

	glBindBuffer(GL_ARRAY_BUFFER, m_fsQuadVBO_ID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18 + sizeof(float) * 12, vboData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 18));

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	glBindVertexArray(GL_NONE);
}

void FrameBuffer::drawFullScreenQuad()
{
	glBindVertexArray(m_fsQuadVAO_ID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(GL_NONE);

}

std::string FrameBuffer::getTag()
{
	return m_tag;
}

unsigned FrameBuffer::getLayer()
{
	return m_layer;
}
