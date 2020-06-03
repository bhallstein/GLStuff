#ifndef __GLStuff_Renderer_2D_Textured
#define __GLStuff_Renderer_2D_Textured

class GLProg;


class Renderer_2D_Textured {
public:
	Renderer_2D_Textured();
	~Renderer_2D_Textured();
	
	bool setUp();
	void setTex(unsigned int);
	void render(float *ortho_mtx);
	
	struct Attribs {
		enum { VertPos, TexCoord };
	};
	struct Uniforms {
		enum { OrthoMatrix, Sampler };
	};
	struct {
		unsigned int vertexPos, texCoord;
	} buffers;
	
	int n_vertices;
	
private:
	GLProg *prog;
	unsigned int vao;
	unsigned int tex;
	
};

#endif

