#ifndef RenderTarget_h
#define RenderTarget_h

#include "GLHelpers.hpp"

struct TextureInfo {
	unsigned int texture;
	Texture::Filtering filtering;
};

struct FBOAttachment {
	TextureInfo tex;
	Framebuffer::AttachmentType type;
};

struct RenderTarget {
	unsigned int fbo;
	std::vector<FBOAttachment> fbo_attachments;
	v3 clearColour = { 0.9, 0.9, 0.9 };
	GLbitfield clear = 0;
	
	void attachAttachments() {
		// Attach dest texture to FBO
		Framebuffer::bind(fbo);
		for (auto &att : fbo_attachments) {
			Framebuffer::attach(att.tex.texture, att.type);
		}
		// TODO: include other attachments
	}
	
	void setUpForRender(int w, int h) {
		Framebuffer::bind(fbo);
		if (clear) {
			glClearColor(clearColour.r, clearColour.g, clearColour.b, 1.0);
			glClear(clear);
		}
	}
};

#endif
