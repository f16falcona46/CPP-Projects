#include "ogl_utils.h"
#include "buffers.h"
#include "state.h"
#include "shaders.h"
#include "update_cube.h"

#include <iostream>
#include <chrono>
#include <signal.h>
#include <time.h>

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

	update_cube_proj(&state, &cube);
	
	timer_t tim;
	int rc;
	struct sigaction sigact; //why you do this POSIX
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	//sigact.sa_handler = [](int sig) {std::cout << "Signal " << sig << "received\n";};
	sigact.sa_handler = SIG_IGN;
	sigaction(SIGALRM, &sigact, nullptr);

	sigset_t sset;
	sigemptyset(&sset);
	sigaddset(&sset, SIGALRM);
	rc = sigprocmask(SIG_BLOCK, &sset, nullptr);
	if (rc) {
		std::cout << "Could not set signal mask.\n";
		return -1;
	}
	
	rc = timer_create(CLOCK_MONOTONIC, nullptr, &tim);
	if (rc) {
		std::cout << "Failed to create timer.\n";
		return -1;
	}
	itimerspec interval;
	interval.it_interval.tv_sec = 0;
	interval.it_interval.tv_nsec = 1000000000 / 120; //60 FPS
	interval.it_value = interval.it_interval;
	rc = timer_settime(tim, 0, &interval, nullptr);
	if (rc) {
		std::cout << "Could not set timer interval.\n";
		return -1;
	}

	int rot_offset = 0;
	int frames = 0;
	auto last_fps_update = std::chrono::system_clock::now();
	while (1) {
		int sig;
		rc = sigwait(&sset, &sig);
		if (rc) {
			std::cout << "Waiting for signal failed.\n";
			return -1;
		}
		if (sig != SIGALRM) {
			continue;
		}

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
		
		for (float x = -3.0f; x <= 3.0f; x += 1.5f) {
			for (float y = -3.0f; y <= 3.0f; y += 1.5f) {
				for (float z = -3.0f; z <= 3.0f; z += 1.5f) {
					update_cube_model(&state, &cube, rot_offset, rot_offset * 2, x, y, z);
					compute_MVP(&cube);
					glUniformMatrix4fv(cubedata.unif_MVP, 1, GL_FALSE, &cube.MVP[0][0]);
					glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
				}
			}
		}

		eglSwapBuffers(state.display, state.surface);
	}
	return 0;
}
