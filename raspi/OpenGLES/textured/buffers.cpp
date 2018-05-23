#include "state.h"
#include "buffers.h"
#include "ogl_utils.h"
#include "OBJ_Loader.h"
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

static const GLushort cube_vertex_indices[] = {
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
	glVertexAttribPointer(data->attr_vertex_pos, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const GLvoid*)0);
	glEnableVertexAttribArray(data->attr_vertex_pos);
	glVertexAttribPointer(data->attr_vertex_texcoord, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const GLvoid*)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(data->attr_vertex_texcoord);
	check();

	glGenBuffers(1, &data->vert_idx_buf);
	check();
}

void init_buffers_cube(const GLES_State* state, GLESData* data)
{
	glGenBuffers(1, &data->vert_buf);
	check();
	
	data->vertices.clear();
	
	objl::Loader Loader;
	bool loadout = Loader.LoadFile("grassy_sample.obj");
	if (loadout) {
		objl::Mesh curMesh = Loader.LoadedMeshes[0];
		data->vertices.clear();
		for (int i = 0; i < curMesh.Vertices.size(); ++i) {
			data->vertices.emplace_back(curMesh.Vertices[i].Position.X);
			data->vertices.emplace_back(curMesh.Vertices[i].Position.Y);
			data->vertices.emplace_back(curMesh.Vertices[i].Position.Z);
			data->vertices.emplace_back(1.0f);
			data->vertices.emplace_back(curMesh.Vertices[i].TextureCoordinate.X);
			data->vertices.emplace_back(curMesh.Vertices[i].TextureCoordinate.Y);
			data->vertices.emplace_back(1.0f);
			data->vertices.emplace_back(curMesh.Vertices[i].Normal.X);
			data->vertices.emplace_back(curMesh.Vertices[i].Normal.Y);
			data->vertices.emplace_back(curMesh.Vertices[i].Normal.Z);
			data->vertices.emplace_back(1.0f);
		}
		data->vert_indexes.clear();
		for (int i = 0; i < curMesh.Indices.size(); ++i) {
			data->vert_indexes.emplace_back(curMesh.Indices[i]);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, data->vert_buf);
	glBufferData(GL_ARRAY_BUFFER, data->vertices.size() * sizeof(GLfloat), data->vertices.data(), GL_STATIC_DRAW);
	if (data->attr_vertex_pos >= 0) {
		glEnableVertexAttribArray(data->attr_vertex_pos);
		glVertexAttribPointer(data->attr_vertex_pos, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (const GLvoid*)0);
	}
	if (data->attr_vertex_texcoord >= 0) {
		glEnableVertexAttribArray(data->attr_vertex_texcoord);
		glVertexAttribPointer(data->attr_vertex_texcoord, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (const GLvoid*)(4 * sizeof(GLfloat)));
	}
	if (data->attr_vertex_normal >= 0) {
		glEnableVertexAttribArray(data->attr_vertex_normal);
		glVertexAttribPointer(data->attr_vertex_normal, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (const GLvoid*)(6 * sizeof(GLfloat)));
	}
	check();

	glGenBuffers(1, &data->vert_idx_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->vert_idx_buf);
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * data->vert_indexes.size(), data->vert_indexes.data(), GL_STATIC_DRAW);
	check();
}
