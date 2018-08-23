#ifndef STATE_H
#define STATE_H

#include "ogl_utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Mesh.h"
#include "GLESData.h"

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

#endif //STATE_H
