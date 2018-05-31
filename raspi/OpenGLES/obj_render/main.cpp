#include "ogl_utils.h"
#include "loadobj.h"
#include "state.h"
#include "shaders.h"
#include "update_cube.h"

#include <iostream>
#include <chrono>
#include <signal.h>
#include <time.h>
#include <vector>

int main()
{
	GLES_State state;
	init_ogl(&state);
	ObjectState cube;
	
	GLESData cubedata;
	cubedata.num_vertices = 5;
	compile_shaders(&state, &cubedata);
	
	std::vector<Mesh> meshes;
	load_obj(&cubedata, &meshes, "White.obj");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, cubedata.vert_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubedata.vert_idx_buf);
	glUseProgram(cubedata.program);

	update_cube_proj(&state, &cube);
	glm::vec3 light_color(100.0f, 100.0f, 100.0f);
	glUniform3fv(cubedata.unif_light_color, 1, &light_color[0]);

	int rot_offset = 0;
	int frames = 0;
	auto last_fps_update = std::chrono::system_clock::now();
	while (1) {
		rot_offset += 2;
		++frames;
		if (frames > 30) {
			auto cur_time = std::chrono::system_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(cur_time - last_fps_update);
			float fps = 1000000.0f / (float)elapsed.count() * (float)frames;
			std::cout << "FPS: " << fps << '\n';
			last_fps_update = cur_time;
			frames = 0;
		}

		int mouse_x, mouse_y;
		get_mouse(&state, &mouse_x, &mouse_y);
		
		update_cube_view(&state, &cube, mouse_x, mouse_y);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::vec4 light_pos(16.0f, 0.0f, 0.0f, 1.0f);
		light_pos = cube.View * light_pos;
		glUniform3fv(cubedata.unif_light_pos, 1, &light_pos[0]);
		glm::vec3 light_color(50.0f, 50.0f, 50.0f);
		glUniform3fv(cubedata.unif_light_color, 1, &light_color[0]);
		
		update_cube_model(&state, &cube, rot_offset, rot_offset * 2, 0, 0, 0, 0.8f);
		compute_MVP_MV(&cube);
		glUniformMatrix4fv(cubedata.unif_MVP, 1, GL_FALSE, &cube.MVP[0][0]);
		glUniformMatrix4fv(cubedata.unif_MV, 1, GL_FALSE, &cube.MV[0][0]);
		glUniformMatrix3fv(cubedata.unif_NormMat, 1, GL_FALSE, &cube.NormMat[0][0]);
		
		for (Mesh m : meshes) {
			bind_mesh(&cubedata, &m);
			glDrawElements(GL_TRIANGLES, cubedata.vert_indexes.size(), GL_UNSIGNED_SHORT, nullptr);
		}
		
		check();
		eglSwapBuffers(state.display, state.surface);
	}
	return 0;
}
