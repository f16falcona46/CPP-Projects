#ifndef SHADERS_H
#define SHADERS_H

#include "ogl_utils.h"
#include "state.h"

const int NUM_LIGHTS = 5;
void compile_shaders(const GLES_State* state, GLESData* data);

#endif //SHADERS_H
