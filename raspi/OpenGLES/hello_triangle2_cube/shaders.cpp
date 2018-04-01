#include "ogl_utils.h"
#include "shaders.h"

static const GLchar* vshader_source = 
	"attribute vec4 vertex;"
	"uniform mat4 MVP;"
	"void main(void) {"
	"	gl_Position = MVP * vertex;"
	"}";

static const GLchar* fshader_source =
	"void main(void) {"
	"	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
	"}";

void compile_shaders(const GLES_State* state, GLESData* data)
{
	data->vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(data->vshader, 1, &vshader_source, nullptr);
	glCompileShader(data->vshader);
	check();

	data->fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(data->fshader, 1, &fshader_source, nullptr);
	glCompileShader(data->fshader);
	check();

	data->program = glCreateProgram();
	glAttachShader(data->program, data->vshader);
	glAttachShader(data->program, data->fshader);
	glLinkProgram(data->program);
	check();
	
	data->attr_vertex = glGetAttribLocation(data->program, "vertex");
	data->unif_MVP = glGetUniformLocation(data->program, "MVP");
	check();

	glGenBuffers(1, &data->buf);
	check();

	glViewport(0, 0, state->screen_width, state->screen_height);
	check();
}
