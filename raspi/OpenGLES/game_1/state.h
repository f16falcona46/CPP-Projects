#ifndef STATE_H
#define STATE_H

#include "ogl_utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Mesh.h"

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
	GLuint unif_tex;
	GLuint unif_light_color;
};

#endif //STATE_H
