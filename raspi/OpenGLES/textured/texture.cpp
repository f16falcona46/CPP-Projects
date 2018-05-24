#include "texture.h"
#include "ogl_utils.h"
#include <gli/gli.hpp>

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

	glm::tvec3<GLsizei> const Extent(Texture.extent());
	GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());

	for(std::size_t Layer = 0; Layer < Texture.layers(); ++Layer)
	for(std::size_t Face = 0; Face < Texture.faces(); ++Face)
	for(std::size_t Level = 0; Level < Texture.levels(); ++Level)
	{
		GLsizei const LayerGL = static_cast<GLsizei>(Layer);
		glm::tvec3<GLsizei> Extent(Texture.extent(Level));
		Target = gli::is_target_cube(Texture.target())
			? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face)
			: Target;

		switch(Texture.target())
		{
		case gli::TARGET_1D_ARRAY:
		case gli::TARGET_2D:
		case gli::TARGET_CUBE:
			if(gli::is_compressed(Texture.format()))
				glCompressedTexSubImage2D(
					Target, static_cast<GLint>(Level),
					0, 0,
					Extent.x,
					Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
					Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
					Texture.data(Layer, Face, Level));
			else
				glTexSubImage2D(
					Target, static_cast<GLint>(Level),
					0, 0,
					Extent.x,
					Texture.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
					Format.External, Format.Type,
					Texture.data(Layer, Face, Level));
			break;
		default: assert(0); break;
		}
	}
	return TextureName;
}*/

GLuint create_texture(char const* Filename)
{
	gli::texture Texture = gli::load(Filename);
	if(Texture.empty())
		return 0;

	gli::gl GL(gli::gl::PROFILE_ES20);
	gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
	GLenum Target = GL.translate(Texture.target());
	assert(gli::is_compressed(Texture.format()) && Target == gli::TARGET_2D);

	GLuint TextureName = 0;
	glGenTextures(1, &TextureName);
	check();
	glBindTexture(Target, TextureName);
	check();
	for(std::size_t Level = 0; Level < Texture.levels(); ++Level)
	{
		glm::tvec3<GLsizei> Extent(Texture.extent(Level));
		glCompressedTexSubImage2D(
			Target, static_cast<GLint>(Level), 0, 0, Extent.x, Extent.y,
			Format.Internal, static_cast<GLsizei>(Texture.size(Level)), Texture.data(0, 0, Level));
		check();
	}

	return TextureName;
}


