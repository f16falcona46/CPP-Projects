#include "ogl_utils.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>

struct ObjectState {
	float angle;
	glm::mat4 Model;
	glm::mat4 View;
	glm::mat4 Projection;
	glm::mat4 MVP;
};

struct GLESData {
	int num_vertices;
	std::vector<GLfloat> vertices;
	GLuint vshader;
	GLuint fshader;
	GLuint program;
	GLint attr_vertex;
	GLuint unif_MVP;
	GLuint buf;
};

/*
static const GLfloat square_vertices[] = {
	-1.0, -1.0, 1.0, 1.0,
	1.0, -1.0, 1.0, 1.0,
	1.0, 1.0, 1.0, 1.0,
	-1.0, 1.0, 1.0, 1.0
};
*/

static const GLchar* vshader_source = 
	"attribute vec4 vertex;"
	"uniform mat4 MVP;"
	"void main(void) {"
	"	gl_Position = MVP * vertex;"
	"}";

static const GLchar* fshader_source =
	"void main(void) {"
	"	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
	"}";

void compile_shaders(const GLES_State* state, GLESData* data)
{
	data->vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(data->vshader, 1, &vshader_source, nullptr);
	glCompileShader(data->vshader);
	check();

	data->fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(data->fshader, 1, &fshader_source, nullptr);
	glCompileShader(data->fshader);
	check();

	data->program = glCreateProgram();
	glAttachShader(data->program, data->vshader);
	glAttachShader(data->program, data->fshader);
	glLinkProgram(data->program);
	check();
	
	data->attr_vertex = glGetAttribLocation(data->program, "vertex");
	data->unif_MVP = glGetUniformLocation(data->program, "MVP");
	check();

	glGenBuffers(1, &data->buf);
	check();

	glViewport(0, 0, state->screen_width, state->screen_height);
	check();
	
	data->vertices.clear();
	auto t = std::chrono::system_clock::now();
	std::time_t seed = std::chrono::system_clock::to_time_t(t);
	std::default_random_engine e(seed);
	std::uniform_real_distribution<> dist(-3, 3);
	for (int i = 0; i < data->num_vertices; ++i) {
		data->vertices.emplace_back(dist(e));
		data->vertices.emplace_back(dist(e));
		data->vertices.emplace_back(dist(e));
		data->vertices.emplace_back(1.0f);
	}
	glBindBuffer(GL_ARRAY_BUFFER, data->buf);
	check();
	glBufferData(GL_ARRAY_BUFFER, data->vertices.size() * sizeof(GLfloat), data->vertices.data(), GL_STATIC_DRAW);
	check();
	glVertexAttribPointer(data->attr_vertex, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	check();
	glEnableVertexAttribArray(data->attr_vertex);
	check();
}

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
