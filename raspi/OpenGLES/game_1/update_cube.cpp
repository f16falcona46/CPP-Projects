#include "update_cube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/transform.hpp>

void update_cube_proj(const GLES_State* state, ObjectState* cube)
{
	cube->Projection = glm::perspective(glm::radians(45.0f),
		(float) state->screen_width / (float) state->screen_height,
		0.1f, 70.0f);
}

void update_cube_model(const GLES_State* state, ObjectState* cube, int x, int y, float x_shift, float y_shift, float z_shift, float size)
{
	const float scale = 5.0f;
	cube->angle_x = ((float) x - (float) state->screen_width / 2.0f)
		/ (float) state->screen_width * scale;
	cube->angle_y = ((float) y - (float) state->screen_height / 2.0f)
		/ (float) state->screen_width * scale;
	cube->Model = glm::scale(glm::vec3(size, size, size));
	cube->Model = glm::rotate(cube->Model, cube->angle_x, glm::vec3(0.0f, 0.0f, 1.0f));
	cube->Model = glm::rotate(cube->Model, cube->angle_y, glm::vec3(1.0f, 0.0f, 0.0f));
	cube->Model = glm::translate(cube->Model, glm::vec3(x_shift, y_shift, z_shift));
}

void update_cube_view(const GLES_State* state, ObjectState* cube, int x, int y)
{
	//const float distance = 40.0f;
	const float distance = 10.0f;
	const float scale = 15.0f;
	cube->angle_x = ((float) x - (float) state->screen_width / 2.0f)
		/ (float) state->screen_width * scale;
	cube->angle_y = ((float) y - (float) state->screen_height / 2.0f)
		/ (float) state->screen_width * scale;
	cube->View = glm::lookAt(
		glm::vec3(distance * std::cos(cube->angle_x) * std::cos(cube->angle_y),
			distance * std::sin(cube->angle_x) * std::cos(cube->angle_y),
			distance * std::sin(cube->angle_y)),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, -1));
}

void compute_MVP_MV(ObjectState* cube)
{
	cube->MVP = cube->Projection * cube->View * cube->Model;
	cube->MV = cube->View * cube->Model;
	cube->NormMat = glm::inverseTranspose(glm::mat3(cube->MV));
}
