#include "ogl_utils.h"
#include "shaders.h"

static const GLchar* vshader_source = 
	"attribute vec4 vertex_pos;"
	"attribute vec2 vertex_texcoord;"
	"attribute vec4 vertex_normal;"
	"uniform mat4 MVP;"
	"varying lowp vec2 uv;"
	"void main(void) {"
	"	gl_Position = MVP * vertex_pos;"
	"	uv = vertex_texcoord;"
	"}";

static const GLchar* fshader_source =
	"uniform sampler2D sampler;"
	"varying lowp vec2 uv;"
	"void main(void) {"
	"	gl_FragColor = texture2D(sampler, uv);"
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
	
	data->attr_vertex_pos = glGetAttribLocation(data->program, "vertex_pos");
	data->attr_vertex_texcoord = glGetAttribLocation(data->program, "vertex_texcoord");
	data->attr_vertex_normal = glGetAttribLocation(data->program, "vertex_normal");
	data->unif_MVP = glGetUniformLocation(data->program, "MVP");
	data->unif_tex = glGetUniformLocation(data->program, "sampler");
	check();

	glViewport(0, 0, state->screen_width, state->screen_height);
	check();
}
