#include "state.h"
#include "buffers.h"
#include "ogl_utils.h"
#include <vector>
#include <random>
#include <chrono>

static const GLshort cube_vertex_indices[] = {
	0, 1, 2,
	2, 1, 3,
	3, 1, 7,
	7, 1, 5,
	4, 1, 0,
	4, 5, 1,
	6, 2, 3,
	6, 3, 7,
	6, 7, 5,
	6, 5, 4,
	6, 0, 2,
	4, 0, 6
};

void init_buffers(const GLES_State* state, GLESData* data)
{
	glGenBuffers(1, &data->vert_buf);
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
		data->vertices.emplace_back(dist(e));
		data->vertices.emplace_back(dist(e));
		data->vertices.emplace_back(dist(e));
		data->vertices.emplace_back(1.0f);
	}
	glBindBuffer(GL_ARRAY_BUFFER, data->vert_buf);
	glBufferData(GL_ARRAY_BUFFER, data->vertices.size() * sizeof(GLfloat), data->vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(data->attr_vertex_pos, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const GLvoid*)0);
	glEnableVertexAttribArray(data->attr_vertex_pos);
	glVertexAttribPointer(data->attr_vertex_color, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const GLvoid*)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(data->attr_vertex_color);
	check();

	glGenBuffers(1, &data->vert_idx_buf);
	check();
}

void init_buffers_cube(const GLES_State* state, GLESData* data)
{
	glGenBuffers(1, &data->vert_buf);
	check();
	
	data->vertices.clear();
	auto t = std::chrono::system_clock::now();
	std::time_t seed = std::chrono::system_clock::to_time_t(t);
	std::default_random_engine e(seed);
	std::uniform_real_distribution<> dist(-3, 3);
	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; ++y) {
			for (int z = -1; z <= 1; ++z) {
				if (x == 0 || y == 0 || z == 0) {
					continue;
				}
				data->vertices.emplace_back(x);
				data->vertices.emplace_back(y);
				data->vertices.emplace_back(z);
				data->vertices.emplace_back(1.0f);
				data->vertices.emplace_back(dist(e));
				data->vertices.emplace_back(dist(e));
				data->vertices.emplace_back(dist(e));
				data->vertices.emplace_back(1.0f);
			}
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, data->vert_buf);
	glBufferData(GL_ARRAY_BUFFER, data->vertices.size() * sizeof(GLfloat), data->vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(data->attr_vertex_pos);
	glVertexAttribPointer(data->attr_vertex_pos, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const GLvoid*)0);
	glEnableVertexAttribArray(data->attr_vertex_color);
	glVertexAttribPointer(data->attr_vertex_color, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const GLvoid*)(4 * sizeof(GLfloat)));
	check();

	glGenBuffers(1, &data->vert_idx_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->vert_idx_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_vertex_indices), cube_vertex_indices, GL_STATIC_DRAW);
	check();
}
