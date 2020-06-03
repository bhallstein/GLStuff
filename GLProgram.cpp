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

GLProgram::GLProgram(const char *vsh, const char *fsh)
{
	_vshPath = new std::string(vsh);
	_fshPath = new std::string(fsh);
}

GLProgram::~GLProgram()
{
	delete (std::string*)_vshPath;
	delete (std::string*)_fshPath;
}

bool GLProgram::load() {
	const char *vsh = ((std::string*)_vshPath)->c_str();
	const char *fsh = ((std::string*)_fshPath)->c_str();
	const char *vShStr = loadShaderString(bundledFilePath(vsh));
	const char *fShStr = loadShaderString(bundledFilePath(fsh));
	
	if (vShStr == NULL || fShStr == NULL)
		return false;
	
	programID = prog_create();
	setAttribLocations();
	
	int compileRes = prog_compileAndLink(programID, vShStr, fShStr);
	if (!compileRes) return false;
	
	prog_use(programID);
	int uniformRes = getUniformLocations();
	if (!uniformRes) return false;
	
	prog_use(0);
	
	return true;
}

