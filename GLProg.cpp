#include "vectors.hpp"
#include "GLProg.hpp"
#include "FilePaths_CPP.hpp"
#include "ShaderStr.hpp"
#include <string>
#include "GLHelpers.hpp"
#include <cstdlib>

void GLProg::compile() {
	state = GLProg::State::NotOK;
	programID = Prog::create();
	
	setAttribLocations(programID, attribs);
	setAttachentLocations(programID, attachments);
	
	char *v_src = loadShaderString(vsh_path.c_str());
	char *f_src = loadShaderString(fsh_path.c_str());
	
	Result compileRes = Prog::compile(programID, v_src, f_src);
	free(v_src);
	free(f_src);
	if (!compileRes.success) {
    return;
  }
	
	Prog::use(programID);
	
	bool uniformRes = getUniformLocations(programID, uniforms);
	if (!uniformRes) {
    return;
  }
	
	setAttribsToUseVBOs(attribs);
	
	state = GLProg::State::OK;
	Prog::use(0);
}

unsigned int GLProg::uniformID(unsigned int internal_id) {
	for (auto &u : uniforms) {
		if (u.userlandID == internal_id) {
			return u.glID;
		}
	}
	return 0;
}

void setAttribLocations(unsigned int prog_id, std::vector<AttribInfo> attribs) {
	for (auto &a : attribs) {
		if (a.name.length() > 0) {
			Prog::set_attrib_location(prog_id, a.glID, a.name);
		}
	}
}

void setAttachentLocations(unsigned int prog_id, std::vector<ColorAttachmentInfo> attachments) {
	for (auto &a : attachments) {
		Prog::set_attachment_location(prog_id, a.glID, a.name);
	}
}

bool getUniformLocations(unsigned int prog_id, std::vector<UniformInfo> &uniforms) {
	bool success = true;
	for (auto &u : uniforms) {
		u.glID = Prog::get_uniform_location(prog_id, u.name);
		if (u.glID == -1) {
			printf("uniform '%s' not found in shader program\n", u.name.c_str());
			success = false;
		}
	}
	return success;
}

void setAttribsToUseVBOs(std::vector<AttribInfo> attribs) {
	for (const auto &a : attribs) {
		if (a.name.length() > 0) {
      Prog::Attrib attrib = {
        a.attrib_type,
        (unsigned int) a.n_components,
        a.instanced,
        a.glID,
        a.vboID,
      };
			Prog::set_attrib_vbo(attrib);
		}
	}
}

