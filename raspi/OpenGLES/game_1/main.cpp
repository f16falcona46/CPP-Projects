#include "ogl_utils.h"
#include "loadobj.h"
#include "state.h"
#include "shaders.h"
#include "update_cube.h"
#include "Entity.h"
#include "OGLEntity.h"
#include "RotatingEntity.h"

#include <iostream>
#include <chrono>
#include <signal.h>
#include <time.h>
#include <vector>
#include <memory>
#include <utility>

#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[])
{
	GLES_State state;
	init_ogl(&state);

	GLESData gldata;
	compile_shaders(&state, &gldata);
	ObjectState mvpstate;
	
	std::srand(std::time(nullptr));
	std::vector<std::unique_ptr<OGLEntity> > entities;
	if (argc > 1) {
		RotatingEntity src(argv[1], gldata);
		for (int i = 0; i < 100; ++i) {
			std::unique_ptr<RotatingEntity> e(new RotatingEntity(src));
			e->x_rate = (std::rand() / (double) RAND_MAX - 0.5) * 2.0;
			e->y_rate = (std::rand() / (double) RAND_MAX - 0.5) * 2.0;
			e->z_rate = (std::rand() / (double) RAND_MAX - 0.5) * 2.0;
			e->scale = 0.6f;
			e->pos = glm::vec3((std::rand() / (double) RAND_MAX - 0.5) * 12.0, (std::rand() / (double) RAND_MAX - 0.5f) * 8.0, (std::rand() / (double) RAND_MAX - 0.5f) * 8.0);
			e->vel = glm::vec3((std::rand() / (double) RAND_MAX - 0.5) * 12.0, (std::rand() / (double) RAND_MAX - 0.5f) * 8.0, (std::rand() / (double) RAND_MAX - 0.5f) * 12.0);
			entities.emplace_back(std::move(e));
		}
	}
	
	glClearColor(0.4f, 0.4f, 0.6f, 1.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	update_cube_proj(&state, &mvpstate);

	int rot_offset = 0;
	int frames = 0;
	auto last_fps_update = std::chrono::system_clock::now();
	auto last_tick = std::chrono::system_clock::now();
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
		
		update_cube_view(&state, &mvpstate, 0, 0);

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
			light_pos[i] = glm::vec3(mvpstate.View * glm::vec4(light_pos[i], 1.0f));
		}
		glUniform3fv(gldata.unif_light_pos, NUM_LIGHTS, &light_pos[0][0]);
		glm::vec3 light_color[NUM_LIGHTS];
		light_color[0] = glm::vec3(10.0f, 10.0f, 10.0f);
		//light_color[1] = glm::vec3(100.0f, 100.0f, 100.0f);
		light_color[2] = glm::vec3(10.0f, 10.0f, 10.0f);
		//light_color[3] = glm::vec3(100.0f, 100.0f, 100.0f);
		for (int i = 4; i < NUM_LIGHTS; ++i) {
			light_color[i] = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		glUniform3fv(gldata.unif_light_color, NUM_LIGHTS, &light_color[0][0]);
		
		float mouse_x_prop = ((float)mouse_x - state.screen_width / 2) / (float) state.screen_height;
		float mouse_y_prop = ((float)mouse_y - state.screen_height / 2) / (float) state.screen_height;
		update_cube_model(&state, &mvpstate, rot_offset, rot_offset * 2, 0.5f);
		
		//e->vel = glm::vec3(mouse_x_prop, mouse_y_prop, 0.0f);
		auto cur_time = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(cur_time - last_tick);
		float tick_time = elapsed.count() / 1000000.0f;
		last_tick = cur_time;
		
		for (auto& e : entities) {
			e->M_rotate = mvpstate.Model;
			e->V = mvpstate.View;
			e->P = mvpstate.Projection;
			if (e->pos.x > 6.0f || e->pos.x < -6.0f) {
				e->vel.x = -e->vel.x;
			}
			if (e->pos.y > 4.0f || e->pos.y < -4.0f) {
				e->vel.y = -e->vel.y;
			}
			if (e->pos.z > 4.0f || e->pos.z < -4.0f) {
				e->vel.z = -e->vel.z;
			}
			e->Tick(tick_time);
			e->Draw();

			//std::cout << e->pos.x << ' ' << e->pos.y << '\n';
		}

		check();
		eglSwapBuffers(state.display, state.surface);
	}
	return 0;
}
