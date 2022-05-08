#include "Renderer_2D_Textured.hpp"
#include "GLProg.hpp"
#include <OpenGL/gl3.h>
#include "CoordinateTypes.hpp"
#include "GLHelpers.hpp"
#include "FilePaths_CPP.hpp"

Renderer_2D_Textured::Renderer_2D_Textured() :
prog(NULL),
vao(0),
tex(0),
n_vertices(0)
{

}


Renderer_2D_Textured::~Renderer_2D_Textured()
{
	vbo_delete(buffers.vertexPos);
	vbo_delete(buffers.texCoord);
	vao_delete(vao);
	if (prog) delete prog;
}


bool Renderer_2D_Textured::setUp() {
	// VAO
	vao = vao_create();
	vao_bind(vao);

	buffers.vertexPos = vbo_create();
	buffers.texCoord  = vbo_create();

	prog = new GLProg;
	prog->vsh_path = bundledFilePath("Shaders/2D_Textured.vsh");
	prog->fsh_path = bundledFilePath("Shaders/2D_Textured.fsh");
	prog->attribs = {
		{ Attribs::VertPos,  buffers.vertexPos, "inVPos",     ATTRTYPE_FLOAT, 2 },
		{ Attribs::TexCoord, buffers.texCoord,  "inTexCoord", ATTRTYPE_FLOAT, 2 }
	};
	prog->uniforms = {
		{ Uniforms::OrthoMatrix, "ortho_matrix" },
		{ Uniforms::Sampler, "uTex"    },
	};

	prog->compile();
	if (prog->state != GLProg::State::OK) {
		printf("oh dear");
		return false;
	}

	vao_bind(0);
	return true;
}

void Renderer_2D_Textured::setTex(unsigned int _tex) {
	tex = _tex;
}

void Renderer_2D_Textured::render(float *ortho_mtx) {
	prog_use(prog->programID);
	vao_bind(vao);

	tx_bind(tex);

	prog_setUniformValue_Mat4(prog->uniformID(Uniforms::OrthoMatrix), ortho_mtx);
	prog_setUniformValue_Int(prog->uniformID(Uniforms::Sampler), 0);

	glDrawArrays(GL_TRIANGLES, 0, n_vertices);

	vao_bind(0);
}
