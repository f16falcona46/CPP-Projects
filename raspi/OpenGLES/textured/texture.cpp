#include "texture.h"
#include "ogl_utils.h"
#include <gli/gli.hpp>
#include <iostream>
#include <GLES2/gl2ext.h>

/// Filename can be KTX or DDS files
/*
GLuint create_texture(char const* Filename)
{
	gli::texture Texture = gli::load(Filename);
	if(Texture.empty())
		return 0;

	gli::gl GL(gli::gl::PROFILE_ES20);
	gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
	GLenum Target = GL.translate(Texture.target());

	GLuint TextureName = 0;
	glGenTextures(1, &TextureName);
	glBindTexture(Target, TextureName);
	check();

	glm::tvec3<GLsizei> const Extent(Texture.extent());
	GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());

	if (gli::is_compressed(Texture.format())) {
		glCompressedTexImage2D(Texture.target(), 0, Format.Internal, Extent.x, Extent.y, 0, static_cast<GLsizei>(Texture.size(0)), Texture.data(0, 0, 0));
	}
	else {
		glTexImage2D(Texture.target(), 0, Format.Internal, Extent.x, Extent.y, 0, static_cast<GLsizei>(Texture.size(0)), Texture.data(0, 0, 0));
	}
	std::cout << glGetError() << '\n';
	check();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return TextureName;
}
*/
GLuint create_texture(char const* Filename)
{
	std::cout << "Loading from " << Filename << '\n';
	gli::texture Texture = gli::load(Filename);
	if(Texture.empty())
		return 0;

	gli::gl GL(gli::gl::PROFILE_ES20);
	gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
	GLenum Target = GL.translate(Texture.target());
	assert(Texture.target() == gli::TARGET_2D);
	assert(gli::is_compressed(Texture.format()));

	GLuint TextureName = 0;
	glGenTextures(1, &TextureName);
	check();
	glBindTexture(Target, TextureName);
	check();
	std::cout << "levels: " << Texture.levels() << " layers: " << Texture.layers() << '\n';
	for(std::size_t Level = 0; Level < Texture.levels(); ++Level)
	{
		glm::tvec3<GLsizei> Extent(Texture.extent(Level));
		glCompressedTexImage2D(Target, Level, Format.Internal, Extent.x, Extent.y, 0, static_cast<GLsizei>(Texture.size(Level)), Texture.data(0, 0, Level));
		check();
		std::cout << Extent.x << ' ' << Extent.y << ' ' << Texture.size(Level) << '\n';
	}

	glTexParameterf(Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	check();
	return TextureName;
}

