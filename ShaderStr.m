//
//  ShaderStr.c
//  OpenGL3.2-Mac
//
//  Created by Ben on 25/09/2013.
//  Copyright (c) 2013 Ben. All rights reserved.
//

#include "ShaderStr.h"
#import <Foundation/Foundation.h>

char* loadShaderString(const char *filepath) {
	NSString *str = [NSString stringWithContentsOfFile:[NSString stringWithFormat:@"%s", filepath]
											  encoding:NSUTF8StringEncoding error:NULL];
	if (str == nil) {
		NSLog(@"loadShaderString: string for file '%s' was nil", filepath);
		return NULL;
	}
	
	return strdup([str UTF8String]);
}
