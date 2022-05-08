#ifndef GLStuff_Renderer_2D_ColourIndexed_h
#define GLStuff_Renderer_2D_ColourIndexed_h

class GLProg;

class Renderer_2D_ColourIndexed {
public:
	Renderer_2D_ColourIndexed(bool dither);
	~Renderer_2D_ColourIndexed();

	bool setUp();
	void render(float *ortho_mtx);

	struct Attribs {
		enum { VertPos, Colour };
	};

	struct Uniforms {
		enum { OrthoMatrix, Sampler_Dithering };
	};

	struct {
		unsigned int vertexPos, colour;
	} buffers;

	int n_vertices;

private:
	GLProg *prog;
	unsigned int vao;

	bool dither;
	unsigned int tex_dither;
};

#endif
