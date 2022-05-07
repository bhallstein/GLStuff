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
	unsigned int glID;          // We can set attrib locs, so the GL ID
	unsigned int vboID;         // == the userland ID
	std::string name;
	attrib_type float_or_int;
	int n_components;
	bool instanced;
};

struct UniformInfo {
	unsigned int userlandID;    // Uniform locs are determined by GL, & are
	std::string name;           // diff. from the userland IDs used to refer
	unsigned int glID;          // to them
};

struct ColorAttachmentInfo {
	unsigned int glID;
	std::string name;
};

struct GLProg {
	std::string vsh_path, fsh_path;
	std::vector<AttribInfo> attribs;
	std::vector<UniformInfo> uniforms;
	std::vector<ColorAttachmentInfo> attachments;
	
	unsigned int programID;
	struct State {
		enum T {
			Vestal, OK, NotOK
		};
	};
	State::T state = State::Vestal;
	
	void compile();
	unsigned int uniformID(unsigned int internal_id);
	
	~GLProg();
};

// These are called automatically by GLProg::compile(),
// but can also be called manually
void setAttribLocations(unsigned int progID, const std::vector<AttribInfo> &);
void setAttachentLocations(unsigned int progID, const std::vector<ColorAttachmentInfo> &);
bool getUniformLocations(unsigned int programID, std::vector<UniformInfo> &uniforms);
void setAttribsToUseVBOs(const std::vector<AttribInfo> &attribs);


#endif
