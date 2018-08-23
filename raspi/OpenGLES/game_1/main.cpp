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

int main(int argc, char* argv[])
{
	GLES_State state;
	init_ogl(&state);
	ObjectState cube;
	
	GLESData cubedata;
	compile_shaders(&state, &cubedata);
	
	std::vector<Mesh> meshes;
	if (argc > 1) {
		load_obj(&cubedata, &meshes, argv[1]);
	}
	else {
		load_obj(&cubedata, &meshes, "White.obj");
	}
	std::cout << "Number of meshes: " << meshes.size() << '\n';
	size_t target_mesh = 999;
	if (argc > 2) {
		int mesh = std::stoi(argv[2]);
		if (mesh < 0) {
			std::cerr << "Mesh must be non-negative.\n";
		}
		else {
			target_mesh = static_cast<size_t>(mesh);
		}
	}

	glClearColor(0.4f, 0.4f, 0.6f, 1.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(cubedata.program);

	update_cube_proj(&state, &cube);

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
		
		//rot_offset = 0;
		int mouse_x, mouse_y;
		get_mouse(&state, &mouse_x, &mouse_y);
		
		update_cube_view(&state, &cube, 0, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::vec3 light_pos[NUM_LIGHTS];
		light_pos[0] = glm::vec3(8.0f, 2.0f, 0.0f);
		light_pos[1] = glm::vec3(0.0f, 32.0f, 0.0f);
		light_pos[2] = glm::vec3(8.0f, 0.0f, -2.0f);
		light_pos[3] = glm::vec3(0.0f, 0.0f, -32.0f);
		for (int i = 4; i < NUM_LIGHTS; ++i) {
			light_pos[i] = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		for (int i = 0; i < NUM_LIGHTS; ++i) {
			light_pos[i] = glm::vec3(cube.View * glm::vec4(light_pos[i], 1.0f));
		}
		glUniform3fv(cubedata.unif_light_pos, NUM_LIGHTS, &light_pos[0][0]);
		glm::vec3 light_color[NUM_LIGHTS];
		light_color[0] = glm::vec3(10.0f, 10.0f, 10.0f);
		//light_color[1] = glm::vec3(100.0f, 100.0f, 100.0f);
		light_color[2] = glm::vec3(10.0f, 10.0f, 10.0f);
		//light_color[3] = glm::vec3(100.0f, 100.0f, 100.0f);
		for (int i = 4; i < NUM_LIGHTS; ++i) {
			light_color[i] = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		glUniform3fv(cubedata.unif_light_color, NUM_LIGHTS, &light_color[0][0]);
		
		float mouse_x_prop = ((float)mouse_x - state.screen_width / 2) / (float) state.screen_height;
		float mouse_y_prop = ((float)mouse_y - state.screen_height / 2) / (float) state.screen_height;
		update_cube_model(&state, &cube, rot_offset, rot_offset, mouse_x_prop, mouse_y_prop, 0, 1.0f);
		compute_MVP_MV(&cube);
		glUniformMatrix4fv(cubedata.unif_MVP, 1, GL_FALSE, &cube.MVP[0][0]);
		glUniformMatrix4fv(cubedata.unif_MV, 1, GL_FALSE, &cube.MV[0][0]);
		glUniformMatrix3fv(cubedata.unif_NormMat, 1, GL_FALSE, &cube.NormMat[0][0]);
		
		for (size_t i = 0; i < meshes.size(); ++i) {
			if (argc > 2) {
				if (target_mesh == i) {
					bind_mesh(&cubedata, &meshes[i]);
					glDrawElements(GL_TRIANGLES, meshes[i].vert_indexes.size(), GL_UNSIGNED_SHORT, nullptr);
				}
			}
			else {
				bind_mesh(&cubedata, &meshes[i]);
				glDrawElements(GL_TRIANGLES, meshes[i].vert_indexes.size(), GL_UNSIGNED_SHORT, nullptr);
			}
		}
		
		check();
		eglSwapBuffers(state.display, state.surface);
	}
	return 0;
}
