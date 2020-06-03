//
//  Shaders.cpp
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#include "GLProgram.h"
#include "FilePaths_CPP.h"
#include "ShaderStr.h"
#include <string>
#include "GLHelpers.h"
#include <cstdlib>

GLProgram::GLProgram(const std::string &_v, const std::string &_f) :
	_vshPath(_v),
	_fshPath(_f)
{
	
}

GLProgram::~GLProgram()
{
	
}

bool GLProgram::compile(
	const std::vector<AttribInfo> &attribs,
	const std::vector<UniformInfo> &uniforms
) {
	const char *shadersDir = "Shaders/";
	const char *vsh = _vshPath.insert(0, shadersDir).c_str();
	const char *fsh = _fshPath.insert(0, shadersDir).c_str();
	const char *vShStr = loadShaderString(bundledFilePath(vsh));
	const char *fShStr = loadShaderString(bundledFilePath(fsh));
	
	if (vShStr == NULL || fShStr == NULL) {
		return false;
	}
	programID = prog_create();
	_setAttribLocations(attribs);
	
	int compileRes = prog_compileAndLink(programID, vShStr, fShStr);
	if (!compileRes) return false;
	
	prog_use(programID);

	int uniformRes = _getUniformLocations(uniforms);
	if (!uniformRes) return false;
	
	_setUpAttribsForDrawing();
	
	prog_use(0);
	
	return true;
}


void GLProgram::_setAttribLocations(
	const std::vector<AttribInfo> &_attribs
) {
	attribs = _attribs;
	for (auto &a : attribs)
		prog_setAttribLocation(programID, a.glID, a.name.c_str());
}

bool GLProgram::_getUniformLocations(
	const std::vector<UniformInfo> &_uniforms
) {
	bool success = true;
	for (auto &u : _uniforms) {
		const char *u_name = u.name.c_str();
		unsigned int loc = prog_uniformLocation(programID, u_name);
		uniform_id_transl_map[u.userlandID] = loc;
		if (loc == -1) {
			printf("uniform \"%s\" not found in shader program\n", u_name);
			success = false;
		}
	}
	return success;
}

void GLProgram::_setUpAttribsForDrawing() {
	for (const auto &a : attribs)
		prog_setAttribToUseVBO(a.glID,
							   a.vboID,
							   a.n_components,
							   a.float_or_int);
}

void GLProgram::setUniform_int(unsigned int userland_id, int x) {
	unsigned int id = uniform_id_transl_map[userland_id];
	prog_setUniformValue_Int(id, x);
}
void GLProgram::setUniform_float(unsigned int userland_id, float x) {
	unsigned int id = uniform_id_transl_map[userland_id];
	prog_setUniformValue_Float(id, x);
}
void GLProgram::setUniform_mat4(unsigned int userland_id, float *value) {
	unsigned int id = uniform_id_transl_map[userland_id];
	prog_setUniformValue_Mat4(id, value);
}
void GLProgram::setUniform_mat3(unsigned int userland_id, float *value) {
	unsigned int id = uniform_id_transl_map[userland_id];
	prog_setUniformValue_Mat3(id, value);
}
void GLProgram::setUniform_vec3(unsigned int userland_id, float *value) {
	unsigned int id = uniform_id_transl_map[userland_id];
	prog_setUniformValue_Vec3(id, value);
}


bool GLProgram::validate() {
	char *res = prog_validate(programID);
	if (res != NULL) {
		printf("%s\n", res);
		free(res);
		return false;
	}
	return true;
}
