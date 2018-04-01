#ifndef STATE_H
#define STATE_H

#include "ogl_utils.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct ObjectState {
	float angle;
	glm::mat4 Model;
	glm::mat4 View;
	glm::mat4 Projection;
	glm::mat4 MVP;
};

struct GLESData {
	int num_vertices;
	std::vector<GLfloat> vertices;
	GLuint vshader;
	GLuint fshader;
	GLuint program;
	GLint attr_vertex_pos;
	GLint attr_vertex_color;
	GLuint unif_MVP;
	GLuint vert_buf;
	GLuint vert_idx_buf;
};

#endif //STATE_H
