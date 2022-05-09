#ifndef TexImage_h
#define TexImage_h

struct TexImage {
	void *data;	  // free() when done
	float w, h;
};
extern struct TexImage null_tex_image;

struct TexImage loadPngTexture(const char *fileName);
	// Pass the full file path
	// - get with bundledFilePath()

int loadCubeMap(unsigned int tex, const char *fileNames[]);
	// RL BT FB

#endif
