#include "GLProg.hpp"
#include "FilePaths_CPP.hpp"
#include "ShaderStr.hpp"
#include <string>
#include "GLHelpers.hpp"
#include <cstdlib>

void GLProg::compile() {
	state = GLProg::State::NotOK;
	programID = prog_create();
	
	setAttribLocations(programID, attribs);
	setAttachentLocations(programID, attachments);
	
	char *v_src = loadShaderString(vsh_path.c_str());
	char *f_src = loadShaderString(fsh_path.c_str());
	
	int compileRes = prog_compileAndLink(programID, v_src, f_src);
	free(v_src);
	free(f_src);
	if (!compileRes) { return; }
	
	prog_use(programID);
	
	bool uniformRes = getUniformLocations(programID, uniforms);
	if (!uniformRes) { return; }
	
	setAttribsToUseVBOs(attribs);
	
	state = GLProg::State::OK;
	prog_use(0);
}

unsigned int GLProg::uniformID(unsigned int internal_id) {
	for (auto &u : uniforms) {
		if (u.userlandID == internal_id) {
			return u.glID;
		}
	}
	return 0;
}


void setAttribLocations(unsigned int programID, const std::vector<AttribInfo> &attribs) {
	for (auto &a : attribs)
		if (a.name.length() > 0)
			prog_setAttribLocation(programID, a.glID, a.name.c_str());
}
void setAttachentLocations(unsigned int programID, const std::vector<ColorAttachmentInfo> &attachments) {
	for (auto &a : attachments)
		prog_setAttachmentLocation(programID, a.glID, a.name.c_str());
}
bool getUniformLocations(unsigned int programID, std::vector<UniformInfo> &uniforms) {
	bool success = true;
	for (auto &u : uniforms) {
		const char *u_name = u.name.c_str();
		u.glID = prog_uniformLocation(programID, u_name);
		if (u.glID == -1) {
			printf("uniform '%s' not found in shader program\n", u_name);
			success = false;
		}
	}
	return success;
}
void setAttribsToUseVBOs(const std::vector<AttribInfo> &attribs) {
	for (const auto &a : attribs)
		if (a.name.length() > 0)
			prog_setAttribToUseVBO(a.glID,
								   a.vboID,
								   a.n_components,
								   a.float_or_int,
								   a.instanced);
}
