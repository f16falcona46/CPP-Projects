#ifndef OGLENTITY_H
#define OGLENTITY_H

#include "Entity.h"
#include "Mesh.h"

class OGLEntity : public Entity {
public:
	template <typename T> OGLEntity(T begin, T end) : meshes(begin, end) {}
protected:
	virtual void draw_internal() override;
	std::vector<Mesh> meshes;
	glm::mat4 M;
	glm::mat4 V;
	glm::mat4 P;
	glm::mat3 NormMat;
};

#endif
