#include "OGLEntity.h"

void OGLEntity::draw_internal()
{
	for (Mesh m : this->meshes) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m.kd_tex);
		glDrawElements(GL_TRIANGLES, m.vert_indexes.size(), GL_UNSIGNED_SHORT, nullptr);
	}
}
