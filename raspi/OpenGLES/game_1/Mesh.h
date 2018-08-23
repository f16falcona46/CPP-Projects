#ifndef MESH_H
#define MESH_H

#include <vector>
#include "GLES2/gl2.h"

struct Mesh {
	std::vector<GLfloat> vertices;
	std::vector<GLshort> vert_indexes;
	GLuint kd_tex;
	GLuint vert_buf;
	GLuint vert_idx_buf;
};

#endif //MESH_H
