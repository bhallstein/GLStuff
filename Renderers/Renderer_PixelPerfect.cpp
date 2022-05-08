#include <OpenGL/gl3.h>
#include "Renderer_PixelPerfect.hpp"
#include "GLProg.hpp"
#include "CoordinateTypes.hpp"
#include "Primitives.hpp"
#include "GLHelpers.hpp"
#include "FilePaths_CPP.hpp"
#include "glm_include.hpp"

Renderer_PixelPerfect::Renderer_PixelPerfect() :
prog(NULL),
vao(0),
n_vertices(0)
{

}


Renderer_PixelPerfect::~Renderer_PixelPerfect()
{
	vbo_delete(buffers.vertexPos);
	vbo_delete(buffers.texCoord);
	vao_delete(vao);
	if (prog) delete prog;
}


bool Renderer_PixelPerfect::setUp() {
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
		{ Uniforms::Sampler,     "uTex"         },
	};

	prog->compile();
	if (prog->state != GLProg::State::OK) {
		printf("oh dear");
		return false;
	}

	vao_bind(0);
	return true;
}

void Renderer_PixelPerfect::render(v2 win_size) {
	prog_use(prog->programID);
	vao_bind(vao);

	glm::mat4 ortho = glm::ortho(0.f, win_size.x, win_size.y, 0.f, 1.f, -1.f);
	prog_setUniformValue_Mat4(Uniforms::OrthoMatrix, mptr(ortho));
	prog_setUniformValue_Int(prog->uniformID(Uniforms::Sampler), 0);

	int i=0;
	unsigned int prev_tex = 0;
	for (const auto &s : sprites) {
		if (i++ == 0 || s.tex != prev_tex)
			tx_bind(s.tex);
		prev_tex = s.tex;

		// Regenerate buffer
		Rectangle2D r = (unitSquare_2D * v2{ 1.0, -1.0 } + 0.5) * s.drawn_size + s.pos;
		vbo_bind(buffers.vertexPos, VBOTYPE_ARRAY);
		vbo_upload(sizeof(v2)*6, r.vertices, VBOTYPE_ARRAY, VBOHINT_STATIC);

		vbo_bind(buffers.texCoord, VBOTYPE_ARRAY);
		Rectangle2D t = unitSquare_texcoords_2D * s.size_in_tex + s.pos_in_tex;
		vbo_upload(sizeof(v2)*6, t.vertices, VBOTYPE_ARRAY, VBOHINT_STATIC);

		n_vertices = 6;
		glDrawArrays(GL_TRIANGLES, 0, n_vertices);
	}

	vao_bind(0);
}


unsigned int Renderer_PixelPerfect::addSprite(Sprite s) {
	sprites.push_back(s);
	return (unsigned int)sprites.size() - 1;
}
