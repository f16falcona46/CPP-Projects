#ifndef MESH_H
#define MESH_H

#include <vector>
#include "GLES2/gl2.h"

struct Mesh {
	size_t num_vert_indexes;
	GLuint kd_tex;
	GLuint vert_buf;
	GLuint vert_idx_buf;
};

inline bool operator==(const Mesh& m1, const Mesh& m2)
{
	return (m1.num_vert_indexes == m2.num_vert_indexes) &&
		(m1.kd_tex == m2.kd_tex) &&
		(m1.vert_buf == m2.vert_buf) &&
		(m1.vert_idx_buf == m2.vert_idx_buf);
}

inline bool operator!=(const Mesh& m1, const Mesh& m2)
{
	return !(m1 == m2);
}

#endif //MESH_H
