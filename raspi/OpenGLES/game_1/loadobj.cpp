#include "state.h"
#include "ogl_utils.h"
#include "loadobj.h"
#include "OBJ_Loader.h"
#include "texture.h"
#include <vector>

void load_mesh(GLESData* data, Mesh* meshd, const objl::Mesh& mesh);

bool load_obj(GLESData* data, std::vector<Mesh>* meshes, const char* filename)
{
	meshes->clear();
	objl::Loader loader;
	bool loaded = loader.LoadFile(filename);
	if (loaded) {
		for (size_t i = 0; i < loader.LoadedMeshes.size(); ++i) {
			meshes->emplace_back();
			load_mesh(data, &(*meshes)[i], loader.LoadedMeshes[i]);
		}
	}
	else {
		std::cerr << "Nothing was loaded.\n";
	}
	return loaded;
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
	std::vector<GLfloat> vertices;
	for (size_t i = 0; i < mesh.Vertices.size(); ++i) {
		vertices.emplace_back(mesh.Vertices[i].Position.X);
		vertices.emplace_back(mesh.Vertices[i].Position.Y);
		vertices.emplace_back(mesh.Vertices[i].Position.Z);
		vertices.emplace_back(1.0f);
		vertices.emplace_back(mesh.Vertices[i].TextureCoordinate.X);
		vertices.emplace_back(mesh.Vertices[i].TextureCoordinate.Y);
		vertices.emplace_back(mesh.Vertices[i].Normal.X);
		vertices.emplace_back(mesh.Vertices[i].Normal.Y);
		vertices.emplace_back(mesh.Vertices[i].Normal.Z);
		vertices.emplace_back(0.0f);
	}

	meshd->num_vert_indexes = mesh.Indices.size();
	std::vector<GLushort> vert_indexes(mesh.Indices.begin(), mesh.Indices.end());

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
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * vert_indexes.size(), vert_indexes.data(), GL_STATIC_DRAW);
	check();
}
