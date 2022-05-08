// Requires Obj-C++
#ifndef FILE_PATHS_OBJC_H
#define FILE_PATHS_OBJC_H

#import <Foundation/Foundation.h>
#import "FilePaths_CPP.hpp"

inline NSString* bundledFilePathAsNSString(const char *f) {
	return [NSString stringWithFormat:@"%@/%s", [[NSBundle mainBundle] resourcePath], f];
}

#endif
