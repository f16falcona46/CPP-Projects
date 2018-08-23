#ifndef GLESDATA_H
#define GLESDATA_H

#include "GLES2/gl2.h"

struct GLESData {
	GLuint vshader;
	GLuint fshader;
	GLuint program;
	GLint attr_vertex_pos;
	GLint attr_vertex_texcoord;
	GLint attr_vertex_normal;
	GLint unif_MVP;
	GLint unif_MV;
	GLint unif_NormMat;
	GLint unif_light_pos;
	GLint unif_tex;
	GLint unif_light_color;
};

#endif //GLESDATA_H
