#include "state.h"
#include "buffers.h"
#include "ogl_utils.h"
#include <vector>
#include <random>
#include <chrono>

void init_buffers(const GLES_State* state, GLESData* data)
{
	glGenBuffers(1, &data->buf);
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
