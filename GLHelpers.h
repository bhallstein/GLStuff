//
//  GLHelpers.h
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 28/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//


// Framebuffers:

#ifndef __GL_HELPERS_H
#define __GL_HELPERS_H

#define bufOffset(i) ((void*)(i))

#ifndef SWAP_TEX
#define SWAP_TEX(x, y) do { unsigned int temp = x; x = y; y = temp; } while (0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum tx_filtering {
	TX_FILTER_LINEAR,
	TX_FILTER_NEAREST
};
enum tx_cubemapface {
	TX_CM_RIGHT,
	TX_CM_LEFT,
	TX_CM_BOTTOM,
	TX_CM_TOP,
	TX_CM_FRONT,
	TX_CM_BACK
};
enum vbo_type {
	VBOTYPE_ARRAY,
	VBOTYPE_ELEMENT_ARRAY
};
enum vbo_hint {
	VBOHINT_STATIC,
	VBOHINT_DYNAMIC
};
enum attrib_type {
	ATTRTYPE_FLOAT,
	ATTRTYPE_INT
};
enum fbo_attachment {
	ATTACH_COLOR,
	ATTACH_DEPTH
};


int fb_create();
void fb_bind(unsigned int fb_id);

// The FB must be bound for attachment alterations
void fb_enableTextureAttachment();
void fb_disableTextureAttachment();
void fb_attachTexture(unsigned int tex_id);
	// The texture does NOT need to be bound to TEXTURE_2D.
	// To detach, use tex_id of 0.

void fb_attachTexture_AsDepth(unsigned int tex_id);
void fb_attachFaceOfCubeMap(unsigned int tex_id, unsigned int face);

int fb_checkOK();


// Textures:

// Limited to using the default texture unit (0)

unsigned int tx_create();
void tx_delete(unsigned int tex_id);
void tx_bind(unsigned int tex_id);
void tx_bindAsCubeMap(unsigned int cm_id);

void tx_setFiltering(enum tx_filtering);
void tx_setRepeat(int repeat);

void tx_upload(int w, int h, void *data, enum tx_filtering filtering);
void tx_uploadDepth(int w, int h, void *data);
void tx_uploadCubeMapFace(int w, int h, void *data, enum tx_cubemapface face);
	// Use data = NULL to allocate empty texture


// VAOs:

unsigned int vao_create();
void vao_bind(unsigned int vao_id);
void vao_delete(unsigned int vao_id);


// VBOs:

unsigned int vbo_create();
void vbo_bind(unsigned int vbo_id, enum vbo_type type);
void vbo_upload(unsigned int n_bytes, void *data, enum vbo_type type, enum vbo_hint hint);
void vbo_reupload(unsigned int n_bytes, int offset, void *data, enum vbo_type type);
void vbo_delete(unsigned int vbo_id);


// Programs:

unsigned int prog_create();
void prog_delete(unsigned int prog_id);
void prog_setAttribLocation(unsigned int program, unsigned int location, const char *name);
void prog_setAttachmentLocation(unsigned int program, unsigned int location, const char *name);
int prog_compileAndLink(unsigned int prog, const char *v_src, const char *f_src);
	// Strings MUST be null-terminated
void prog_use(unsigned int program);

char* prog_validate(unsigned int program);
	// Returns NULL if passed validation, otherwise a malloc'd string
	
int prog_uniformLocation(unsigned int program, const char *uniform_name);
void prog_setUniformValue_Int(int uniform_loc, int value);
void prog_setUniformValue_Float(int uniform_loc, float value);
void prog_setUniformValue_Vec3(int uniform_loc, float *values);
void prog_setUniformValue_Vec4(int uniform_loc, float *values);
void prog_setUniformValue_Mat3(int uniform_loc, float *values);
void prog_setUniformValue_Mat4(int uniform_loc, float *values);

void prog_setAttribToUseVBO(unsigned int attrib_loc,
							unsigned int vbo_id,
							int attrib_components,
							enum attrib_type attrtype,
							int instanced);
	// This binds the vbo, enabled the attrib atrray & sets its pointer
	// NB attrib array enabling is per-VAO, or global if no VAO is bound.
	// So when not using VAOs, you should disable it after use with prog_disableAttrib()
	
void prog_setAttrbDivisor(unsigned int attrib_loc, unsigned int divisor);
void prog_disableAttrib(unsigned int attrib_loc);
	// ...but only when not using VAOs.
	
#ifdef __cplusplus
}
#endif

#endif
