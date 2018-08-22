#ifndef OGL_UTILS_H
#define OGL_UTILS_H

#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include <cassert>

struct GLES_State {
   uint32_t screen_width;
   uint32_t screen_height;
// OpenGL|ES objects
   EGLDisplay display;
   EGLSurface surface;
   EGLContext context;
};

#ifndef check
#define check() assert(glGetError() == 0)
#endif //check

void showlog(GLint shader);
void showprogramlog(GLint shader);
void init_ogl(GLES_State *state);
int get_mouse(const GLES_State* state, int* outx, int* outy);

#endif //OGL_UTILS_H
