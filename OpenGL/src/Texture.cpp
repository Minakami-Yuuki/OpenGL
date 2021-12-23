#include "Texture.h"

#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string path)
	:m_RendererID(0) ,m_FilePath(path), m_LocalBuffer(nullptr),
	 m_Width(0), m_Height(0), m_BPP(0)
{
	// in OpenGL: it reads PNG from bottom to top
	// So, we need th flip the picture vertically.
	stbi_set_flip_vertically_on_load(1);

	/*	param: (To load the picture position)
	*	*filename, *x, *y, *channels_in_file, desire_channels
	*	*channels_in_file: the method to write the file (BPP == byte per pixel)
	*	desire_channels: how to show the file (R G B A == 4)
	*/
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 3);

	GLCall(glGenTextures(1, &m_RendererID));
	// Binding Texture
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	/* if not to create : there will be showed a black screen */
	// create the filter to min or mag the picture (scale)
	// Parameteri == parameter integer
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	// S & T == X & Y (To match the color in edge of picture)
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// set the TextureImage format
	/*	param:
	*		level: 0 (first)
	*		internalformat:	GL_RGB8	(the texture save in openGL (it needs to show how many bits pre pixel))
	*		format:			GL_RGB	(the texture save in Memory (it no needs to show how many bits pre pixel))
	*		*pixel:			where load the texture ?
	*/
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer));
	// Unbinding Texture
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	// release storage place
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot /* = 0*/) const
{
	// choose what slot you want to bind the Texture
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}