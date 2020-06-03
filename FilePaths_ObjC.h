//
//  FilePaths.h
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

// This refers to NSString, so cannot be imported by pure C++.
// However, it can be imported by Obj-C++.
// C++ code will want to use the _CPP version anyway

#ifndef __FILE_PATHS_OBJC_H
#define __FILE_PATHS_OBJC_H

#ifdef __cplusplus
extern "C" {
#endif

	NSString* bundledFilePathAsNSString(const char *filepath);
	
#ifdef __cplusplus
}
#endif


#endif
