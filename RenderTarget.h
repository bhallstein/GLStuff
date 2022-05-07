//
//  RenderTarget.h
//
//  Created by Ben on 03/01/2016.
//  Copyright © 2016 Ben. All rights reserved.
//

#ifndef RenderTarget_h
#define RenderTarget_h

#include "GLHelpers.h"

struct TextureInfo {
	unsigned int texture;
	tx_filtering filtering;
};
struct FBOAttachment {
	TextureInfo tex;
	fbo_attachment type;
};

struct RenderTarget {
	unsigned int fbo;
	std::vector<FBOAttachment> fbo_attachments;
	col3 clearColour = { 0.9, 0.9, 0.9 };
	GLbitfield clear = 0;
	
	void attachAttachments() {
		// Attach dest texture to FBO
		fb_bind(fbo);
		for (auto &att : fbo_attachments) {
			if (att.type == ATTACH_COLOR)
				fb_attachTexture(att.tex.texture);
			else if (att.type == ATTACH_DEPTH)
				fb_attachTexture_AsDepth(att.tex.texture);
		}
		// ...
		// TODO: modify to include other attachments
	}
	
	void setUpForRender(int w, int h) {
		fb_bind(fbo);
		
		if (clear) {
			glClearColor(clearColour.r, clearColour.g, clearColour.b, 1.0);
			glClear(clear);
		}
	}
	
};


#endif
