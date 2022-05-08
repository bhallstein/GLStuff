#include "Renderer_2D_ColourIndexed.hpp"
#include "GLProg.hpp"
#include <OpenGL/gl3.h>
#include "CoordinateTypes.hpp"
#include "Primitives.hpp"
#include "FilePaths_CPP.hpp"

Renderer_2D_ColourIndexed::Renderer_2D_ColourIndexed(bool _dither) :
prog(NULL),
n_vertices(0),
dither(_dither)
{

}
Renderer_2D_ColourIndexed::~Renderer_2D_ColourIndexed()
{
	vbo_delete(buffers.vertexPos);
	vbo_delete(buffers.colour);
	vao_delete(vao);
	tx_delete(tex_dither);
	if (prog) delete prog;
}

bool Renderer_2D_ColourIndexed::setUp() {
	vao = vao_create();
	vao_bind(vao);

	buffers.vertexPos = vbo_create();
	buffers.colour = vbo_create();

	prog = new GLProg;
	prog->vsh_path = bundledFilePath("Shaders/2D_ColourIndexed.vsh");
	prog->fsh_path = bundledFilePath("Shaders/2D_ColourIndexed.fsh");
	prog->attribs = {
		{ Attribs::VertPos, buffers.vertexPos, "inVPos",   ATTRTYPE_FLOAT, 2, false },
		{ Attribs::Colour,  buffers.colour,    "inColour", ATTRTYPE_FLOAT, 4, false },
	};
	prog->uniforms = {
		{ Uniforms::OrthoMatrix, "ortho_matrix" },
	};
	if (dither) {
		prog->fsh_path = bundledFilePath("Shaders/2D_ColourIndexed_Dithered.fsh");
		prog->uniforms.push_back({ Uniforms::Sampler_Dithering, "uTex" });

		tex_dither = tx_create();
		tx_bind(tex_dither);
		tx_setRepeat(true);
		tx_setFiltering(TX_FILTER_NEAREST);

		static const char pattern[] = {
			0, 32,  8, 40,  2, 34, 10, 42,   /* 8x8 Bayer ordered dithering  */
			48, 16, 56, 24, 50, 18, 58, 26,  /* pattern.  Each input pixel   */
			12, 44,  4, 36, 14, 46,  6, 38,  /* is scaled to the 0..63 range */
			60, 28, 52, 20, 62, 30, 54, 22,  /* before looking in this table */
			3, 35, 11, 43,  1, 33,  9, 41,   /* to determine the action.     */
			51, 19, 59, 27, 49, 17, 57, 25,
			15, 47,  7, 39, 13, 45,  5, 37,
			63, 31, 55, 23, 61, 29, 53, 21
		};
		glTexImage2D(GL_TEXTURE_2D,
								 0,        // mipmap
								 GL_RED,   // internal format
								 8,        // w
								 8,        // h
								 0,        // border
								 GL_RED,   // given format
								 GL_UNSIGNED_BYTE,  // given type
								 pattern);
		tx_bind(0);
	}

	prog->compile();
	if (prog->state != GLProg::State::OK) {
		printf("Renderer_2D_ColourIndexed: Failed to compile program\n");
		vao_bind(0);
		return false;
	}

	Rectangle2D r = unitSquare * 2.0;
	col4 colours[] = {
		{ 1.0, 0.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 },
		{ 1.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 }, { 1.0, 1.0, 0.0, 1.0 }
	};

	vbo_bind(buffers.vertexPos, VBOTYPE_ARRAY);
	vbo_upload(sizeof(v2)*6, r.vertices, VBOTYPE_ARRAY, VBOHINT_STATIC);
	vbo_bind(buffers.colour, VBOTYPE_ARRAY);
	vbo_upload(sizeof(col4)*6, colours, VBOTYPE_ARRAY, VBOHINT_STATIC);

	n_vertices = 6;

	vao_bind(0);
	return true;
}
void Renderer_2D_ColourIndexed::render(float *ortho_mtx) {
	glDisable(GL_DEPTH_TEST);
	vao_bind(vao);

	prog_use(prog->programID);
	prog_setUniformValue_Mat4(prog->uniformID(Uniforms::OrthoMatrix), ortho_mtx);

	if (dither) {
		tx_bind(tex_dither);
		prog_setUniformValue_Int(prog->uniformID(Uniforms::Sampler_Dithering), 0);
	}

	glDrawArrays(GL_TRIANGLES, 0, 6);
	vao_bind(0);
}
