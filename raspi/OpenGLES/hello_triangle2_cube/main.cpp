#include "ogl_utils.h"
#include "buffers.h"
#include "state.h"
#include "shaders.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>

void update_cube_state(const GLES_State* state, ObjectState* cube, int x, int y, float x_shift, float y_shift, float z_shift)
{
	const float distance = 20.0f;
	const float scale = 5.0f;
	cube->angle_x = ((float) x - (float) state->screen_width / 2.0f)
		/ (float) state->screen_width * scale;
	cube->angle_y = ((float) y - (float) state->screen_height / 2.0f)
		/ (float) state->screen_width * scale;
	cube->Projection = glm::perspective(glm::radians(45.0f),
		(float) state->screen_width / (float) state->screen_height,
		0.1f, 70.0f);
	/*
	cube->View = glm::lookAt(
		glm::vec3(distance * std::cos(cube->angle_x) * std::cos(cube->angle_y),
			distance * std::sin(cube->angle_x) * std::cos(cube->angle_y),
			distance * std::sin(cube->angle_y)),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 1));
	*/
	cube->View = glm::lookAt(
		glm::vec3(distance,
			distance,
			distance),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 1));
	cube->Model = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	cube->Model = glm::translate(cube->Model, glm::vec3(x_shift, y_shift, z_shift));
	cube->Model = glm::rotate(cube->Model, cube->angle_x, glm::vec3(0.0f, 0.0f, 1.0f));
	cube->Model = glm::rotate(cube->Model, cube->angle_y, glm::vec3(1.0f, 0.0f, 0.0f));
	cube->MVP = cube->Projection * cube->View * cube->Model;
}

int main()
{
	GLES_State state;
	init_ogl(&state);
	ObjectState cube;
	
	GLESData cubedata;
	cubedata.num_vertices = 5;
	compile_shaders(&state, &cubedata);
	init_buffers_cube(&state, &cubedata);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, cubedata.vert_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubedata.vert_idx_buf);
	glUseProgram(cubedata.program);
	while (1) {
		int mouse_x, mouse_y;
		get_mouse(&state, &mouse_x, &mouse_y);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for (float x = -6; x <= 6; x += 3.0) {
			for (float y = -6; y <= 6; y += 3.0) {
				for (float z = -6; z <= 6; z += 3.0) {
					update_cube_state(&state, &cube, mouse_x, mouse_y, x, y, z);
					glUniformMatrix4fv(cubedata.unif_MVP, 1, GL_FALSE, &cube.MVP[0][0]);
					glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
				}
			}
		}

		glFlush();
		glFinish();
		eglSwapBuffers(state.display, state.surface);
		check();
	}
	return 0;
}
