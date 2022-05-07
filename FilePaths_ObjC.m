//
//  FilePaths.m
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "FilePaths_ObjC.h"

NSString* bundledFilePathAsNSString(const char *f) {
	return [NSString stringWithFormat:@"%@/%s", [[NSBundle mainBundle] resourcePath], f];
}
