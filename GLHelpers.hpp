#ifndef GLHelpers_h
#define GLHelpers_h

#include <OpenGL/gl3.h>
#include <string>
#include "vectors.hpp"

// #define bufOffset(i) ((void *)(i))

// #ifndef SWAP_TEX
// #define SWAP_TEX(x, y) \
//   do                   \
//   {                    \
//     auto temp = x;     \
//     x = y;             \
//     y = temp;          \
//   } while (0)
// #endif

// result
// ------------------------------------

struct Result {
  bool success;
  std::string message;
};


// Framebuffer
// ------------------------------------

struct Framebuffer {
	enum AttachmentType {
		Color,
		Depth,
		CubeMap,
	};

	static int create() {
		GLuint fboName;
		glGenFramebuffers(1, &fboName);
		return fboName;
	}

	static void bind(unsigned int fb_id) {
		glBindFramebuffer(GL_FRAMEBUFFER, fb_id);
	}

	static int check() {
		auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		return status == GL_FRAMEBUFFER_COMPLETE;
	}

	static void enable_write_to_texture() { // FB must be bound
		unsigned int att = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &att);
	}

	static void disable_write_to_texture() { // FB must be bound
		unsigned int att = GL_NONE;
		glDrawBuffers(1, &att);
	}

	static void attach(unsigned int tex_id, AttachmentType attachment, int face = 0) {
		// N.B. - The texture does NOT need to be bound to TEXTURE_2D
		//      - To detach, use tex_id 0
		auto gl_textarget = attachment == CubeMap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
		auto gl_attachment = attachment == Color ? GL_COLOR_ATTACHMENT0 :
		attachment == Depth ? GL_DEPTH_ATTACHMENT :
		GL_TEXTURE_CUBE_MAP;
		glFramebufferTexture2D(GL_FRAMEBUFFER,
													 gl_attachment,
													 gl_textarget,
													 tex_id,
													 0);
	}
};


// Texture
// ------------------------------------

struct Texture {
	enum Filtering {
		FilterLinear,
		FilterNearest,
	};
	enum CubeMapFace {
		CubeMapRight,
		CubeMapLeft,
		CubeMapBottom,
		CubeMapTop,
		CubeMapFront,
		CubeMapBack,
	};

	static unsigned int create() {
		unsigned int texID;
		glGenTextures(1, &texID);
		return texID;
	}

	static void destroy(unsigned int tex_id) {
		glDeleteTextures(1, &tex_id);
	}

	static void bind(unsigned int tex_unit, unsigned int tex_id) {
    glActiveTexture(GL_TEXTURE0 + tex_unit);
		glBindTexture(GL_TEXTURE_2D, tex_id);
	}

	static void bind_cube_map(unsigned int tex_unit, unsigned int cm_id) {
    glActiveTexture(GL_TEXTURE0 + tex_unit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cm_id);
	}

	static void set_filtering(Filtering f) {
		GLenum gl_filt = (f == FilterLinear ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filt);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filt);
	}

	static void set_repeat(int repeat) {
		GLenum gl_repeat = (repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_repeat);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_repeat);
	}

	static void upload(int w, int h, void* data, Filtering f) {
		set_filtering(f);
		glTexImage2D(GL_TEXTURE_2D,
								 0,
								 GL_RGBA,
								 w, h,
								 0,
								 GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV,
								 data);
	}

	static void upload_depth(int w, int h, void* data) {
		Texture::set_filtering(FilterNearest);
		glTexImage2D(GL_TEXTURE_2D,
								 0,
								 GL_DEPTH_COMPONENT,
								 w, h,
								 0,
								 GL_DEPTH_COMPONENT, GL_UNSIGNED_INT,
								 data);
	}

	static void upload_cube_map_face(int w, int h, void* data, CubeMapFace face) {
		// Use data = NULL to allocate empty texture
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
};


// VAO
// ------------------------------------

struct VAO {
	static unsigned int create() {
		unsigned int vao_id;
		glGenVertexArrays(1, &vao_id);
		return vao_id;
	}

	static void destroy(unsigned int vao_id) {
		glDeleteVertexArrays(2, &vao_id);
	}

	static void bind(unsigned int va_id) {
		glBindVertexArray(va_id);
	}
};


// VBO
// ------------------------------------

struct VBO {
	enum VBOType {
		Array,
		ElementArray,
	};
  
	enum VBOHint {
		Static,
		Dynamic,
	};

	static unsigned int create() {
		unsigned int vbo_id;
		glGenBuffers(1, &vbo_id);
		return vbo_id;
	}

	static void destroy(unsigned int vbo_id) {
		glDeleteBuffers(1, &vbo_id);
	}

	static void bind(unsigned int vbo_id, VBOType type) {
		GLenum gl_target = (type == Array ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER);
		glBindBuffer(gl_target, vbo_id);
	}

	static void upload(unsigned long n_bytes, void* data, VBOType type, VBOHint hint) {
		GLenum gl_target = (type == Array ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER);
		GLenum gl_hint = (hint == Dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBufferData(gl_target, n_bytes, data, gl_hint);
	}

  template<class T>
  static void upload(std::vector<T> data, VBOType type, VBOHint hint) {
    upload(sizeof(T) * data.size(), &data[0], type, hint);
  }

	static void reupload(unsigned int n_bytes, int offset, void* data, VBOType type) {
		GLenum gl_target = (type == Array ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER);
		glBufferSubData(gl_target, 0, n_bytes, data);
	}
};


// Prog
// ------------------------------------

struct Prog {
	enum AttribType {
		Float,
		Int,
	};
  
  struct Attrib {
    AttribType type;
    unsigned int n_components;
    bool instanced;
    // Internal
    unsigned int attrib_id;
    unsigned int vbo;
  };
  
  struct Uniform {
    enum UniformType {
      Float, Int,
      V2, V3, V4,
      I2, I3, I4,
      M3, M4,
      Quat,
      Sampler2D, SamplerCube,
    } type;
    // Internal
    unsigned int uniform_id;
  };
  
  struct Attachment {
    unsigned int attachment_id;
  };

	static unsigned int create() {
		return glCreateProgram();
	}

	static void destroy(unsigned int prog_id) {
		glDeleteProgram(prog_id);
	}

	static void use(unsigned int program) {
		glUseProgram(program);
	}

	static void set_attrib_location(unsigned int program, unsigned int location, std::string name) {
		glBindAttribLocation(program, location, name.c_str());
	}

	static void set_attachment_location(unsigned int program, unsigned int location, std::string name) {
		glBindFragDataLocation(program, location, name.c_str());
	}

	static Result compile(unsigned int prog, std::string vsrc, std::string fsrc) {
		GLint log_length, status, link_status;
		const char* vsrc_char = vsrc.c_str();
		const char* fsrc_char = fsrc.c_str();
    std::string message;

		// Compile vertex shader
		GLuint vsh = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vsh, 1, &vsrc_char, NULL);
		glCompileShader(vsh);

		// Check compiled
		glGetShaderiv(vsh, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 0) {
			GLchar* log = (GLchar*) malloc(log_length);
			glGetShaderInfoLog(vsh, log_length, &log_length, log);
      message = log;
			free(log);
		}
		glGetShaderiv(vsh, GL_COMPILE_STATUS, &status);
		if (status == 0) {
      message += "Failed to compile vertex shader:\n";
      message += vsrc_char;
      return {false, message};
		}

		// Compile fragment shader
		GLuint fsh = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fsh, 1, &fsrc_char, NULL);
		glCompileShader(fsh);

		// Check compiled
		glGetShaderiv(fsh, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 0) {
			GLchar* log = (GLchar*)malloc(log_length);
			glGetShaderInfoLog(fsh, log_length, &log_length, log);
      message += "Fragment shader compile log:\n";
      message += log;
			free(log);
		}
		glGetShaderiv(fsh, GL_COMPILE_STATUS, &status);
		if (status == 0) {
      message += "Failed to compile fragment shader:\n";
      message += fsrc_char;
      return {false, message};
		}

		// Link program
		glAttachShader(prog, vsh);
		glAttachShader(prog, fsh);
		glLinkProgram(prog);

		// Check linked OK
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 0) {
			GLchar* log = (GLchar*)malloc(log_length);
			glGetProgramInfoLog(prog, log_length, &log_length, log);
      message += "Program link log:\n";
      message += log;
			free(log);
		}

		glGetProgramiv(prog, GL_LINK_STATUS, &link_status);
		if (link_status == 0) {
      return {false, "Failed to link program"};
		}

		glValidateProgram(prog);
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 0) {
			GLchar* log = (GLchar*)malloc(log_length);
			glGetProgramInfoLog(prog, log_length, &log_length, log);
      message += "Program validate log:\n";
      message += log;
			free(log);
		}

		glGetProgramiv(prog, GL_VALIDATE_STATUS, &link_status);
		if (link_status == 0) {
      return {false, "Program failed to validate, continuing..."};
		}

		glDeleteShader(vsh);
		glDeleteShader(fsh);
    return {true, message};
	}

	static std::string validate(unsigned int program) {
		// Returns error or "success"
		int log_length;
		int link_status;

		glValidateProgram(program);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 0) {
			char* msg = (char*)malloc(log_length);
			glGetProgramInfoLog(program, log_length, &log_length, msg);
			std::string s = msg;
			free(msg);
			return s;
		}

		glGetProgramiv(program, GL_VALIDATE_STATUS, &link_status);
		if (link_status == 0) {
			return "Failed to validate program (no log found)!";
		}

		return "success";
	}

	static int get_uniform_location(unsigned int prog_id, std::string uniform_name) {
		return glGetUniformLocation(prog_id, uniform_name.c_str());
	}

	static void set_uniform_int(int uniform_loc, int value) {
		glUniform1i(uniform_loc, value);
	}

	static void set_uniform_float(int uniform_loc, float value) {
		glUniform1f(uniform_loc, value);
	}

  static void set_uniform_vec2(int uniform_loc, v2 vec) {
    glUniform2fv(uniform_loc, 1, glm::value_ptr(vec));
  }
  
	static void set_uniform_vec3(int uniform_loc, v3 vec) {
		glUniform3fv(uniform_loc, 1, glm::value_ptr(vec));
	}

	static void set_uniform_vec4(int uniform_loc, v4 vec) {
		glUniform4fv(uniform_loc, 1, glm::value_ptr(vec));
	}
  
  static void set_uniform_ivec2(int uniform_loc, i2 vec) {
    glUniform2iv(uniform_loc, 1, glm::value_ptr(vec));
  }
  
  static void set_uniform_ivec3(int uniform_loc, i3 vec) {
    glUniform3iv(uniform_loc, 1, glm::value_ptr(vec));
  }
  
  static void set_uniform_ivec4(int uniform_loc, i4 vec) {
    glUniform4iv(uniform_loc, 1, glm::value_ptr(vec));
  }
  
	static void set_uniform_mat3(int uniform_loc, m3 mat) {
		glUniformMatrix3fv(uniform_loc, 1, GL_FALSE, glm::value_ptr(mat));
	}

	static void set_uniform_mat4(int uniform_loc, m4 mat) {
		glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, glm::value_ptr(mat));
	}
  
  static void set_uniform_quat(int uniform_loc, quat q) {
    glUniform4fv(uniform_loc, 1, glm::value_ptr(q));
  }

	static void set_attrib_vbo(Attrib attrib) {
		// - Bind the vbo, enable the attrib & set its pointer
		// - NB attrib enabling is per-VAO, or global if no VAO is bound.
		// - So when not using VAOs, you should disable it after use with disableAttrib()
    //   ...but only when not using VAOs.
		GLenum gl_attrib_type = (attrib.type == Float ? GL_FLOAT : GL_INT);
		glBindBuffer(GL_ARRAY_BUFFER, attrib.vbo);
		glEnableVertexAttribArray(attrib.attrib_id);
		glVertexAttribPointer(attrib.attrib_id,     // index
													attrib.n_components,  // size (components per vertex)
													gl_attrib_type,       // type
													GL_FALSE,             // normalized
													0,                    // stride
													0);                   // offset/pointer
		if (attrib.instanced) {
			set_attrib_divisor(attrib.attrib_id, 1);
		}
	}

	static void set_attrib_divisor(unsigned int attrib_loc, unsigned int divisor) {
		glVertexAttribDivisor(attrib_loc, divisor);
	}

	static void disable_attrib(unsigned int attrib_loc) {
		glDisableVertexAttribArray(attrib_loc);
	}
};

#endif
