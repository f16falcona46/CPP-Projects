#ifndef LOADOBJ_H
#define LOADOBJ_H

#include "state.h"
#include <vector>

void load_obj(GLESData* data, std::vector<Mesh>* meshes, const char* filename);
void bind_mesh(GLESData* data, const Mesh* mesh);

#endif //LOADOBJ_H