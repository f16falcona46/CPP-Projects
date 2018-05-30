#ifndef STATE_H
#define STATE_H

#include "ogl_utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct ObjectState {
	float angle_x;
	float angle_y;
	glm::mat4 Model;
	glm::mat4 View;
	glm::mat4 Projection;
	glm::mat4 MVP;
	glm::mat4 MV;
	glm::mat3 NormMat;
	glm::vec3 light_position;
};

struct GLESData {
	int num_vertices;
	std::vector<GLfloat> vertices;
	std::vector<GLushort> vert_indexes;
	GLuint vshader;
	GLuint fshader;
	GLuint program;
	GLint attr_vertex_pos;
	GLint attr_vertex_texcoord;
	GLint attr_vertex_normal;
	GLuint unif_MVP;
	GLuint unif_MV;
	GLuint unif_NormMat;
	GLuint unif_light_pos;
	GLuint unif_light_color;
	GLuint unif_tex;
	GLuint vert_buf;
	GLuint vert_idx_buf;
	GLuint kd_tex;
};

#endif //STATE_H
