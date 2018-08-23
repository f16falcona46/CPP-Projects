#ifndef OGLENTITY_H
#define OGLENTITY_H

#include <string>
#include "Entity.h"
#include "Mesh.h"
#include "GLESData.h"

class OGLEntity : public Entity {
public:
	template <typename T> OGLEntity(T begin, T end, const GLESData& GLdata) : meshes(begin, end), data(GLdata) {}
	OGLEntity(const std::string& obj_filename, const GLESData& GLdata);
	OGLEntity() : loaded(false) {}
	glm::mat4 M_rotate;
	glm::mat4 M;
	glm::mat4 V;
	glm::mat4 P;
	glm::mat3 NormMat;
protected:
	virtual void draw_internal() override;
	virtual void tick_internal(float time) override;
	std::vector<Mesh> meshes;
	GLESData data;
	bool loaded;
};

#endif
