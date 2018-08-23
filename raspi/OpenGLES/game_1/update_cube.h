#include "ogl_utils.h"
#include "state.h"

void update_cube_proj(const GLES_State* state, ObjectState* cube);

void update_cube_model(const GLES_State* state, ObjectState* cube, int x, int y, float size);

void update_cube_view(const GLES_State* state, ObjectState* cube, int x, int y);

void compute_MVP_MV(ObjectState* cube);
