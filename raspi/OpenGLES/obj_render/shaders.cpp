#include "ogl_utils.h"
#include "shaders.h"
#include <iostream>

static void getShaderInfoLog(GLint shader)
{
	char log[1024];
	glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
	std::cout << log << '\n';
}

static void getProgramInfoLog(GLint program)
{
	char log[1024];
	glGetProgramInfoLog(program, sizeof(log), nullptr, log);
	std::cout << log << '\n';
}

#define PER_VERTEX (1)

#ifdef PER_VERTEX
static const GLchar* vshader_source = 
	"const int NUM_LIGHTS = 5;"
	"attribute vec4 vertex_pos;"
	"attribute vec2 vertex_texcoord;"
	"attribute vec4 vertex_normal;"
	"uniform mat3 NormMat;"
	"uniform mat4 MVP;"
	"uniform mat4 MV;"
	"uniform vec3 light_pos[NUM_LIGHTS];"
	"uniform vec3 light_color[NUM_LIGHTS];"
	"varying lowp vec2 uv;"
	"varying lowp vec3 light;"
	"void main(void) {"
	"	light = vec3(0.0);"
	"	for (int i = 0; i < NUM_LIGHTS; ++i) {"
	"		vec3 mv_vertex = vec3(MV * vertex_pos);"
	"		vec3 mv_normal = normalize(vec3(MV * vertex_normal));"
	"		float distance = length(light_pos[i] - mv_vertex);"
	"		vec3 light_dir = normalize(light_pos[i] - mv_vertex);"
	"		float diffuse = max(dot(mv_normal, light_dir), 0.1);"
	"		diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));"
	"		light += light_color[i] * diffuse;"
	"	}"
	"	gl_Position = MVP * vertex_pos;"
	"	uv = vertex_texcoord;"
	"}";

static const GLchar* fshader_source =
	"uniform sampler2D sampler;"
	"varying lowp vec2 uv;"
	"varying lowp vec3 light;"
	"void main(void) {"
	"	gl_FragColor = texture2D(sampler, uv) * vec4(light, 1.0);"
	"}";
#else
#ifdef NO_TEXTURE
static const GLchar* vshader_source = 
	"const int NUM_LIGHTS = 5;"
	"attribute vec4 vertex_pos;"
	"attribute vec2 vertex_texcoord;"
	"attribute vec4 vertex_normal;"
	"uniform mat3 NormMat;"
	"uniform mat4 MVP;"
	"uniform mat4 MV;"
	"uniform vec3 light_pos[NUM_LIGHTS];"
	"uniform vec3 light_color[NUM_LIGHTS];"
	"varying lowp vec2 uv;"
	"varying lowp vec3 light;"
	"void main(void) {"
	"	light = vec3(0.0);"
	"	for (int i = 0; i < NUM_LIGHTS; ++i) {"
	"		vec3 mv_vertex = vec3(MV * vertex_pos);"
	"		vec3 mv_normal = normalize(vec3(MV * vertex_normal));"
	"		float distance = length(light_pos[i] - mv_vertex);"
	"		vec3 light_dir = normalize(light_pos[i] - mv_vertex);"
	"		float diffuse = max(dot(mv_normal, light_dir), 0.1);"
	"		diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));"
	"		light += light_color[i] * diffuse;"
	"	}"
	"	gl_Position = MVP * vertex_pos;"
	"	uv = vertex_texcoord;"
	"}";

static const GLchar* fshader_source =
	"uniform sampler2D sampler;"
	"varying lowp vec2 uv;"
	"varying lowp vec3 light;"
	"void main(void) {"
	"	gl_FragColor = vec4(light, 1.0);"
	"}";
#else
static const GLchar* vshader_source = 
	"attribute vec4 vertex_pos;"
	"attribute vec2 vertex_texcoord;"
	"attribute vec4 vertex_normal;"
	"uniform mat4 MVP;"
	"uniform mat4 MV;"
	"uniform mat3 NormMat;"
	"varying vec2 uv;"
	"varying vec3 v_mv_vertex_pos;"
	"varying vec3 v_mv_vertex_normal;"
	"void main(void) {"
	"	v_mv_vertex_normal = normalize(vec3(MV * vertex_normal));"
	"	v_mv_vertex_pos = vec3(MV * vertex_pos);"
	"	gl_Position = MVP * vertex_pos;"
	"	uv = vertex_texcoord;"
	"}";

static const GLchar* fshader_source =
	"const int NUM_LIGHTS = 5;"
	"uniform sampler2D sampler;"
	"uniform vec3 light_pos[NUM_LIGHTS];"
	"uniform vec3 light_color[NUM_LIGHTS];"
	"varying vec2 uv;"
	"varying vec3 v_mv_vertex_pos;"
	"varying vec3 v_mv_vertex_normal;"
	"void main(void) {"
	"	vec3 light = vec3(0.0);"
	"	for (int i = 0; i < NUM_LIGHTS; ++i) {"
	"		float distance = length(light_pos[i] - v_mv_vertex_pos);"
	"		vec3 light_dir = normalize(light_pos[i] - v_mv_vertex_pos);"
	"		float diffuse = max(dot(v_mv_vertex_normal, light_dir), 0.1);"
	"		diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));"
	"		light += light_color[i] * diffuse;"
	"	}"
	"	gl_FragColor = texture2D(sampler, uv) * vec4(light, 1.0);"
	"}";
#endif //FLAT
#endif //PER_VERTEX

void compile_shaders(const GLES_State* state, GLESData* data)
{
	data->vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(data->vshader, 1, &vshader_source, nullptr);
	glCompileShader(data->vshader);
	check();
	getShaderInfoLog(data->vshader);

	data->fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(data->fshader, 1, &fshader_source, nullptr);
	glCompileShader(data->fshader);
	check();
	getShaderInfoLog(data->fshader);

	data->program = glCreateProgram();
	glAttachShader(data->program, data->vshader);
	glAttachShader(data->program, data->fshader);
	glLinkProgram(data->program);
	check();

	getProgramInfoLog(data->program);
	
	data->attr_vertex_pos = glGetAttribLocation(data->program, "vertex_pos");
	data->attr_vertex_texcoord = glGetAttribLocation(data->program, "vertex_texcoord");
	data->attr_vertex_normal = glGetAttribLocation(data->program, "vertex_normal");
	data->unif_MVP = glGetUniformLocation(data->program, "MVP");
	data->unif_MV = glGetUniformLocation(data->program, "MV");
	data->unif_NormMat = glGetUniformLocation(data->program, "NormMat");
	data->unif_light_pos = glGetUniformLocation(data->program, "light_pos");
	data->unif_light_color = glGetUniformLocation(data->program, "light_color");
	data->unif_tex = glGetUniformLocation(data->program, "sampler");
	check();

	glViewport(0, 0, state->screen_width, state->screen_height);
	check();
}
