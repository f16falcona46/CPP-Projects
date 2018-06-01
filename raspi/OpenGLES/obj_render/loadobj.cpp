#include "state.h"
#include "ogl_utils.h"
#include "loadobj.h"
#include "OBJ_Loader.h"
#include "texture.h"
#include <vector>

void load_mesh(GLESData* data, Mesh* meshd, const objl::Mesh& mesh);

void load_obj(GLESData* data, std::vector<Mesh>* meshes, const char* filename)
{
	meshes->clear();
	objl::Loader Loader;
	bool loadout = Loader.LoadFile(filename);
	if (loadout) {
		for (int i = 0; i < Loader.LoadedMeshes.size(); ++i) {
			meshes->emplace_back();
			load_mesh(data, &(*meshes)[i], Loader.LoadedMeshes[i]);
		}
	}
	else {
		std::cerr << "Nothing was loaded.\n";
	}
}

void bind_mesh(GLESData* data, const Mesh* mesh)
{
	glUseProgram(data->program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->kd_tex);
	glUniform1i(data->unif_tex, 0);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vert_buf);
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->vert_idx_buf);
}
	

void load_mesh(GLESData* data, Mesh* meshd, const objl::Mesh& mesh)
{
	meshd->vertices.clear();
	for (int i = 0; i < mesh.Vertices.size(); ++i) {
		meshd->vertices.emplace_back(mesh.Vertices[i].Position.X);
		meshd->vertices.emplace_back(mesh.Vertices[i].Position.Y);
		meshd->vertices.emplace_back(mesh.Vertices[i].Position.Z);
		meshd->vertices.emplace_back(1.0f);
		meshd->vertices.emplace_back(mesh.Vertices[i].TextureCoordinate.X);
		meshd->vertices.emplace_back(mesh.Vertices[i].TextureCoordinate.Y);
		meshd->vertices.emplace_back(mesh.Vertices[i].Normal.X);
		meshd->vertices.emplace_back(mesh.Vertices[i].Normal.Y);
		meshd->vertices.emplace_back(mesh.Vertices[i].Normal.Z);
		meshd->vertices.emplace_back(0.0f);
	}
	meshd->vert_indexes.clear();
	for (int i = 0; i < mesh.Indices.size(); ++i) {
		meshd->vert_indexes.emplace_back(mesh.Indices[i]);
	}
	meshd->kd_tex = create_texture(mesh.MeshMaterial.map_Kd.c_str());
	check();
	glActiveTexture(GL_TEXTURE0);
	check();
	glBindTexture(GL_TEXTURE_2D, meshd->kd_tex);
	check();
	glUseProgram(data->program);
	glUniform1i(data->unif_tex, 0);
	check();
	std::cout << "verts, idx: " << mesh.Vertices.size() << ' ' << mesh.Indices.size() << '\n';
	std::cout << "map_Kd: " << mesh.MeshMaterial.map_Kd << '\n';
		
	glGenBuffers(1, &meshd->vert_buf);
	check();
	glBindBuffer(GL_ARRAY_BUFFER, meshd->vert_buf);
	check();
	glBufferData(GL_ARRAY_BUFFER, meshd->vertices.size() * sizeof(GLfloat), meshd->vertices.data(), GL_STATIC_DRAW);
	check();
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

	glGenBuffers(1, &meshd->vert_idx_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshd->vert_idx_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * meshd->vert_indexes.size(), meshd->vert_indexes.data(), GL_STATIC_DRAW);
	check();
}
