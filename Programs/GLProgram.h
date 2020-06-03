//
//  Shaders.h
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#ifndef __GL_PROGRAMS_H
#define __GL_PROGRAMS_H

#include <string>
#include <vector>
#include <map>
#include "GLHelpers.h"

struct AttribInfo {
	unsigned int glID;			// We can set attrib locs, so the GL ID
	unsigned int vboID;			// == the userland ID
	std::string name;
	attrib_type float_or_int;
	int n_components;
};
struct UniformInfo {
	unsigned int userlandID;		// Uniform locs are determined by GL, & are
	std::string name;			// diff. from the userland IDs used to refer
};								// to them


class GLProgram {
public:
	GLProgram(
		const std::string &vertShPath,
		const std::string &fragShPath
	);
	~GLProgram();
		// 0. Create the VBOs used for the program's attribs
	
	bool compile(
		const std::vector<AttribInfo> &attribs,
		const std::vector<UniformInfo> &uniforms
	);
		// 1. Compile the program
		//    (also sets attrib locatiotns, creates a private map of user => gl uniform IDs,
		//     and binds attrib locations to VBOs.)
	
	bool validate();
	
	unsigned int id() { return programID; }
	
	void setUniform_int(unsigned int id, int val);
	void setUniform_float(unsigned int id, float value);
	void setUniform_mat4(unsigned int id, float *value);
	void setUniform_mat3(unsigned int id, float *value);
	void setUniform_vec3(unsigned int id, float *value);
	
private:
	std::string _vshPath, _fshPath;
	unsigned int programID;
	
	std::vector<AttribInfo> attribs;
	std::map<unsigned int, unsigned int> uniform_id_transl_map;
	
	void _setAttribLocations(
		const std::vector<AttribInfo> &attribs
	);
	bool _getUniformLocations(
		const std::vector<UniformInfo> &uniforms
	);
	void _setUpAttribsForDrawing();
};


#endif
