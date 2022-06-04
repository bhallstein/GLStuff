#ifndef Renderer_h
#define Renderer_h

#include <map>
#include "GLHelpers.hpp"
// #include "Primitives.hpp"


struct Renderer {
  unsigned int vao = -1;
  unsigned int program_id = -1;
  
  std::map<std::string, Prog::Attrib> attribs;
  std::map<std::string, Prog::Uniform> uniforms;
  std::map<std::string, Prog::Attachment> attachments;
  std::string vertex_shader;
  std::string fragment_shader;
  bool instanced = false;
  bool dither = false;   // If true, dither texture will use texture unit 0
  
  // Internal
  unsigned int dither_tex;

  enum DrawMode {
    Points,
    LineStrip,
    LineLoop,
    Lines,
    TriangleStrip,
    TriangleFan,
    Triangles,
  };
  
	~Renderer() {
    // NB: Renderer does not create or destroy VAOs, VBOs, or Textures
		if (program_id != -1) {
      Prog::destroy(program_id);
		}
	}

	Result init() {
    using Uniform = Prog::Uniform;
    using Attrib = Prog::Attrib;
    
		VAO::bind(vao);
    program_id = Prog::create();

    if (dither) {
      dither_tex = upload_dither_texture();
      uniforms["u_dither_tex"] = Uniform{Uniform::Sampler2D};
    }
    
    int i=0;
    for (auto &item : attribs) {
      std::string name = item.first;
      Attrib &attrib = item.second;
      attrib.attrib_id = i++;
      Prog::set_attrib_location(program_id, attrib.attrib_id, name);
    }
//    i = 0;
//    for (auto &item : attachments) {
//      item.second.attachment_id = i++;
//      Prog::set_attachment_location(program_id, item.second.attachment_id, item.first);
//    }

    Result compile_result = Prog::compile(program_id, vertex_shader, fragment_shader);
    if (!compile_result.success) {
      return {false, "Renderer: failed to compile program"};
    }

    for (auto &item : uniforms) {
      std::string name = item.first;
      Uniform &uniform = item.second;
      uniform.uniform_id = Prog::get_uniform_location(program_id, name);
    }
    
    for (const auto &attrib : attribs) {
      Prog::set_attrib_vbo(attrib.second);
    }

//    Primitive2D r = Rectangle2D() * v2{2., 2.};
//		std::vector<v4> colours = {
//			{ 1.0, 0.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 },
//			{ 0.0, 0.0, 1.0, 1.0 }, { 1.0, 1.0, 0.0, 1.0 }, { 1.0, 0.0, 0.0, 1.0 }
//		};

//		VBO::bind(buffers.vertexPos, VBO::Array);
//		VBO::upload(r, VBO::Array, VBO::Static);
//
//    VBO::bind(buffers.colour, VBO::Array);
//		VBO::upload(colours, VBO::Array, VBO::Static);

		VAO::bind(0);
    return {true};
	}
  
  // NB: Call Prog::use() before these helper funcs
  void set_uniform_value(Prog::Uniform uniform, int value) {
    Prog::set_uniform_int(uniform.uniform_id, value);
  }
  void set_uniform_value(Prog::Uniform uniform, float value) {
    Prog::set_uniform_float(uniform.uniform_id, value);
  }
  void set_uniform_value(Prog::Uniform uniform, v2 value) {
    Prog::set_uniform_vec2(uniform.uniform_id, value);
  }
  void set_uniform_value(Prog::Uniform uniform, v3 value) {
    Prog::set_uniform_vec3(uniform.uniform_id, value);
  }
  void set_uniform_value(Prog::Uniform uniform, v4 value) {
    Prog::set_uniform_vec4(uniform.uniform_id, value);
  }
  void set_uniform_value(Prog::Uniform uniform, i2 value) {
    Prog::set_uniform_ivec2(uniform.uniform_id, value);
  }
  void set_uniform_value(Prog::Uniform uniform, i3 value) {
    Prog::set_uniform_ivec3(uniform.uniform_id, value);
  }
  void set_uniform_value(Prog::Uniform uniform, i4 value) {
    Prog::set_uniform_ivec4(uniform.uniform_id, value);
  }
  void set_uniform_value(Prog::Uniform uniform, m3 value) {
    Prog::set_uniform_mat3(uniform.uniform_id, value);
  }
  void set_uniform_value(Prog::Uniform uniform, m4 value) {
    Prog::set_uniform_mat4(uniform.uniform_id, value);
  }
  void set_uniform_value(Prog::Uniform uniform, quat value) {
    Prog::set_uniform_quat(uniform.uniform_id, value);
  }

  void render(DrawMode draw_mode, VBO::VBOType arrays_or_elements) {
    // glDisable(GL_DEPTH_TEST);   // 2D renderers should do this.
                                   // Consider an array of enable / disables in renderer confif
		VAO::bind(vao);
		Prog::use(program_id);
		if (dither) {
			Texture::bind(0, dither_tex);
      set_uniform_value(uniforms["u_dither_tex"], 0);
		}

		glDrawArrays(GL_TRIANGLES, 0, 6);
		VAO::bind(0);
	}

  unsigned int upload_dither_texture() {
    unsigned int tex = Texture::create();
    Texture::bind(0, tex);
    Texture::set_repeat(true);
    Texture::set_filtering(Texture::FilterNearest);

    static const char dither_pattern[] = {
      0,  32,  8, 40,  2, 34, 10, 42,  /* 8x8 Bayer ordered dithering  */
      48, 16, 56, 24, 50, 18, 58, 26,  /* pattern.  Each input pixel   */
      12, 44,  4, 36, 14, 46,  6, 38,  /* is scaled to the 0..63 range */
      60, 28, 52, 20, 62, 30, 54, 22,  /* before looking in this table */
      3,  35, 11, 43,  1, 33,  9, 41,  /* to determine the action.     */
      51, 19, 59, 27, 49, 17, 57, 25,
      15, 47,  7, 39, 13, 45,  5, 37,
      63, 31, 55, 23, 61, 29, 53, 21
    };

    glTexImage2D(GL_TEXTURE_2D,
                 0,        // mipmap
                 GL_RED,   // internal format
                 8,        // w
                 8,        // h
                 0,        // border
                 GL_RED,   // given format
                 GL_UNSIGNED_BYTE,  // given type
                 dither_pattern);
    Texture::bind(0, 0);
    return tex;
  }
};

#endif
