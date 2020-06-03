//
//  Scene.h
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 29/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#ifndef __SCENE_H
#define __SCENE_H

class Scene {
public:
	Scene(unsigned int _defaultFBO);
	virtual ~Scene();
	
	virtual void render() = 0;
	
protected:
	unsigned int defaultFBO;

};

#endif
