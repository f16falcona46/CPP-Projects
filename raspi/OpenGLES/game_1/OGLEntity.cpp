#include "OGLEntity.h"

#include <iostream>
#include <glm/gtc/matrix_inverse.hpp>

#include "ogl_utils.h"
#include "texture.h"
#include "loadobj.h"

OGLEntity::OGLEntity(const std::string& obj_filename, const GLESData& GLdata) : meshes(), data(GLdata)
{
	this->loaded = load_obj(&this->data, &this->meshes, obj_filename.c_str());
}

void OGLEntity::draw_internal()
{
	if (this->loaded) {
		glm::mat4 MV = this->V * this->M;
		glm::mat4 MVP = this->P * MV;
		this->NormMat = glm::inverseTranspose(glm::mat3(MV));
		glUniformMatrix4fv(this->data.unif_MVP, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(this->data.unif_MV, 1, GL_FALSE, &MV[0][0]);
		glUniformMatrix3fv(this->data.unif_NormMat, 1, GL_FALSE, &this->NormMat[0][0]);
		for (Mesh m : this->meshes) {
			glUseProgram(this->data.program);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m.kd_tex);
			glBindBuffer(GL_ARRAY_BUFFER, m.vert_buf);
			if (this->data.attr_vertex_pos >= 0) {
				glEnableVertexAttribArray(this->data.attr_vertex_pos);
				glVertexAttribPointer(this->data.attr_vertex_pos, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (const GLvoid*)0);
			}
			if (this->data.attr_vertex_texcoord >= 0) {
				glEnableVertexAttribArray(this->data.attr_vertex_texcoord);
				glVertexAttribPointer(this->data.attr_vertex_texcoord, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (const GLvoid*)(4 * sizeof(GLfloat)));
			}
			if (this->data.attr_vertex_normal >= 0) {
				glEnableVertexAttribArray(this->data.attr_vertex_normal);
				glVertexAttribPointer(this->data.attr_vertex_normal, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (const GLvoid*)(6 * sizeof(GLfloat)));
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.vert_idx_buf);
			glDrawElements(GL_TRIANGLES, m.num_vert_indexes, GL_UNSIGNED_SHORT, nullptr);
		}
	}
}

void OGLEntity::tick_internal(float time)
{
	this->M = glm::translate(glm::mat4(1.0f), this->pos) * this->M_rotate;
}
