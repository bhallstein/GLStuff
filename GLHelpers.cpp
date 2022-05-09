#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <OpenGL/gl3.h>
#include "GLHelpers.hpp"

// Framebuffers
// --------------------------------------

int fb_create() {
	GLuint fboName;
	glGenFramebuffers(1, &fboName);
	return fboName;
}

void fb_bind(unsigned int fb) {
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
}

void fb_enableTextureAttachment() {
	unsigned int att = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &att);
}
void fb_disableTextureAttachment() {
	unsigned int att = GL_NONE;
	glDrawBuffers(1, &att);
}

void fb_attachTexture(unsigned int tex_id) {
	glFramebufferTexture2D(GL_FRAMEBUFFER,
												 GL_COLOR_ATTACHMENT0,
												 GL_TEXTURE_2D,
												 tex_id,
												 0);
}

void fb_attachTexture_AsDepth(unsigned int tex_id) {
	glFramebufferTexture2D(GL_FRAMEBUFFER,
												 GL_DEPTH_ATTACHMENT,
												 GL_TEXTURE_2D,
												 tex_id,
												 0);
}

void fb_attachFaceOfCubeMap(unsigned int cubemap_id, unsigned int face) {
	glFramebufferTexture2D(GL_FRAMEBUFFER,
												 GL_COLOR_ATTACHMENT0,
												 GL_TEXTURE_CUBE_MAP,
												 cubemap_id,
												 0);
}

int fb_checkOK() {
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Error: RTT framebuffer incomplete");
		return 0;
	}
	return 1;
}



// Textures
// --------------------------------------

unsigned int tx_create() {
	unsigned int texID;
	glGenTextures(1, &texID);
	return texID;
}
void tx_delete(unsigned int tex_id) {
	glDeleteTextures(1, &tex_id);
}

void tx_bind(unsigned int tex_id) {
	glBindTexture(GL_TEXTURE_2D, tex_id);
}
void tx_bindAsCubeMap(unsigned int cm_id) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, cm_id);
}

void tx_setFiltering(enum tx_filtering f) {
	GLenum gl_filt = (f == TX_FILTER_LINEAR ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filt);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filt);
}
void tx_setRepeat(int repeat) {
	GLenum gl_repeat = (repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_repeat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_repeat);
}

void tx_upload(int w, int h, void *data, enum tx_filtering filtering) {
	tx_setFiltering(filtering);
	glTexImage2D(GL_TEXTURE_2D,
							 0,
							 GL_RGBA,
							 w, h,
							 0,
							 GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV,
							 data);
}
void tx_uploadDepth(int w, int h, void *data) {
	tx_setFiltering(TX_FILTER_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,
							 0,
							 GL_DEPTH_COMPONENT,
							 w, h,
							 0,
							 GL_DEPTH_COMPONENT, GL_UNSIGNED_INT,
							 data);
}
void tx_uploadCubeMapFace(int w, int h, void *data, enum tx_cubemapface face) {
	GLenum gl_face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + (int)face;
	glTexImage2D(gl_face,
							 0,
							 GL_RGBA,
							 w, h,
							 0,
							 GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV,
							 data);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Technically we only need to do this once for the entire cube map.
}


// VAOs
// --------------------------------------

unsigned int vao_create() {
	unsigned int vao_id;
	glGenVertexArrays(1, &vao_id);
	return vao_id;
}
void vao_bind(unsigned int id) {
	glBindVertexArray(id);
}
void vao_delete(unsigned int vao_id) {
	glDeleteVertexArrays(2, &vao_id);
}



// VBOs
// --------------------------------------

unsigned int vbo_create() {
	unsigned int vbo_id;
	glGenBuffers(1, &vbo_id);
	return vbo_id;
}

void vbo_bind(unsigned int vbo_id, enum vbo_type type) {
	GLenum gl_target = (type == VBOTYPE_ARRAY ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(gl_target, vbo_id);
}

void vbo_upload(unsigned int n_bytes, void *data, enum vbo_type type, enum vbo_hint hint) {
	GLenum gl_target = (type == VBOTYPE_ARRAY ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER);
	GLenum gl_hint = (hint == VBOHINT_DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBufferData(gl_target,
							 n_bytes,
							 data,
							 gl_hint);
}

void vbo_reupload(unsigned int bytes, int offset, void *data, enum vbo_type type) {
	GLenum gl_target = (type == VBOTYPE_ARRAY ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER);
	glBufferSubData(gl_target,
									0,
									bytes,
									data);
}

void vbo_delete(unsigned int vbo_id) {
	glDeleteBuffers(1, &vbo_id);
}


// Programs
// --------------------------------------

unsigned int prog_create() {
	return glCreateProgram();
}
void prog_delete(unsigned int prog_id) {
	glDeleteProgram(prog_id);
}

void prog_setAttribLocation(unsigned int program, unsigned int location, const char *name) {
	glBindAttribLocation(program, location, name);
}
void prog_setAttachmentLocation(unsigned int program, unsigned int location, const char *name) {
	glBindFragDataLocation(program, location, name);
}

int prog_compileAndLink(unsigned int prog, const char *v_src, const char *f_src) {
	GLint logLength, compileStatus, linkStatus;

	// Compile vertex shader
	GLuint vSh = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vSh, 1, &v_src, NULL);
	glCompileShader(vSh);

	// Check compiled
	glGetShaderiv(vSh, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar *log = (GLchar*) malloc(logLength);
		glGetShaderInfoLog(vSh, logLength, &logLength, log);
		printf("Vertex shader compile log: %s\n", log);
		free(log);
	}
	glGetShaderiv(vSh, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == 0) {
		printf("Failed to compile vertex shader:\n%s\n", v_src);
		return 0;
	}

	// Compile fragment shader
	GLuint fSh = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fSh, 1, (const GLchar **) &f_src, NULL);
	glCompileShader(fSh);

	// Check compiled
	glGetShaderiv(fSh, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar *log = (GLchar*) malloc(logLength);
		glGetShaderInfoLog(fSh, logLength, &logLength, log);
		printf("Fragment shader compile log: %s\n", log);
		free(log);
	}
	glGetShaderiv(fSh, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == 0) {
		printf("Failed to compile fragment shader:\n%s\n", f_src);
		return 0;
	}

	// Link program
	glAttachShader(prog, vSh);
	glAttachShader(prog, fSh);
	glLinkProgram(prog);

	// Check linked OK
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar *log = (GLchar*)malloc(logLength);
		glGetProgramInfoLog(prog, logLength, &logLength, log);
		printf("Program link log:\n%s\n", log);
		free(log);
	}

	glGetProgramiv(prog, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == 0) {
		printf("Failed to link program");
		return 0;
	}

	glValidateProgram(prog);
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		GLchar *log = (GLchar*)malloc(logLength);
		glGetProgramInfoLog(prog, logLength, &logLength, log);
		printf("Program validate log:\n%s\n", log);
		free(log);
	}

	glGetProgramiv(prog, GL_VALIDATE_STATUS, &linkStatus);
	if (linkStatus == 0) {
		printf("Program failed to validate, continuing...");
		return 0;
	}

	glDeleteShader(vSh);
	glDeleteShader(fSh);

	return 1;
}

void prog_use(unsigned int program) { glUseProgram(program); }

char* prog_validate(unsigned int program) {
	int logLength, linkStatus;

	glValidateProgram(program);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		char *msg = (char*) malloc(logLength);
		glGetProgramInfoLog(program, logLength, &logLength, msg);
		return msg;
	}

	glGetProgramiv(program, GL_VALIDATE_STATUS, &linkStatus);
	if (linkStatus == 0)
		return strdup("Failed to validate program (no log found)!\n");

	return NULL;
}

int prog_uniformLocation(unsigned int prog, const char *uniform_name) {
	return glGetUniformLocation(prog, uniform_name);
}
void prog_setUniformValue_Int(int uniform_loc, int value) {      glUniform1i(uniform_loc, value); }
void prog_setUniformValue_Float(int uniform_loc, float value) {  glUniform1f(uniform_loc, value); }
void prog_setUniformValue_Vec3(int uniform_loc, float *values) { glUniform3fv(uniform_loc, 1, values); }
void prog_setUniformValue_Vec4(int uniform_loc, float *values) { glUniform4fv(uniform_loc, 1, values); }
void prog_setUniformValue_Mat3(int uniform_loc, float *values) { glUniformMatrix3fv(uniform_loc, 1, GL_FALSE, values); }
void prog_setUniformValue_Mat4(int uniform_loc, float *values) { glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, values); }

void prog_setAttribToUseVBO(unsigned int attrib_loc,
														unsigned int vbo_id,
														int attrib_components,
														enum attrib_type attrtype,
														int instanced) {
	GLenum gl_attrib_type = (attrtype == ATTRTYPE_FLOAT ? GL_FLOAT : GL_INT);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glEnableVertexAttribArray(attrib_loc);
	glVertexAttribPointer(attrib_loc,           // index
												attrib_components,    // size (components per vertex)
												gl_attrib_type,       // type
												GL_FALSE,             // normalized
												0,                    // stride
												0);                   // offset/pointer
	if (instanced)
		glVertexAttribDivisor(attrib_loc, 1);
}

void prog_setAttrbDivisor(unsigned int attrib_loc, unsigned int divisor) {
	glVertexAttribDivisor(attrib_loc, divisor);
}

void prog_disableAttrib(unsigned int attrib_loc) {
	glDisableVertexAttribArray(attrib_loc);
}
