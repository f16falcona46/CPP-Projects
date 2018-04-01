#include "ogl_utils.h"
#include "buffers.h"
#include "state.h"
#include "shaders.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>

void update_cube_state(const GLES_State* state, ObjectState* cube)
{
	int x, y;
	get_mouse(state, &x, &y);
	cube->angle = ((float) x - (float) state->screen_width / 2.0f)
		/ (float) state->screen_width * 2.0f;
	cube->Projection = glm::perspective(glm::radians(45.0f),
		(float) state->screen_width / (float) state->screen_height,
		0.1f, 100.0f);
	cube->View = glm::lookAt(
		glm::vec3(5.0f * std::cos(cube->angle), 5.0f * std::sin(cube->angle), 4.0f),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 1));
	cube->Model = glm::mat4(1.0f);
	cube->MVP = cube->Projection * cube->View * cube->Model;
}

int main()
{
	GLES_State state;
	init_ogl(&state);
	ObjectState cube;
	
	GLESData cubedata;
	cubedata.num_vertices = 500;
	compile_shaders(&state, &cubedata);
	init_buffers(&state, &cubedata);

	while (1) {
		update_cube_state(&state, &cube);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		check();
		
		glBindBuffer(GL_ARRAY_BUFFER, cubedata.buf);
		check();
		glUseProgram(cubedata.program);
		check();
		glUniformMatrix4fv(cubedata.unif_MVP, 1, GL_FALSE, &cube.MVP[0][0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, cubedata.num_vertices);
		check();

		glFlush();
		glFinish();
		check();
		eglSwapBuffers(state.display, state.surface);
		check();
	}
	return 0;
}
