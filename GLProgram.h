//
//  Shaders.h
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#ifndef __GL_PROGRAMS_H
#define __GL_PROGRAMS_H

class GLProgram {
public:
	GLProgram(const char *vertShPath, const char *fragShPath);
	~GLProgram();
	
	bool load();	// Create, compile & link the program
	
	unsigned int programID;
	
	virtual void setUpAttribsForDrawing() = 0;
	
protected:
	virtual void setAttribLocations() = 0;
	virtual bool getUniformLocations() = 0;
	
private:
	void *_vshPath, *_fshPath;
	
};


#endif