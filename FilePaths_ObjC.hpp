#ifndef FilePaths_Objc_h
#define FilePaths_Objc_h

#import <Foundation/Foundation.h>
#import "FilePaths_CPP.hpp"

inline NSString* bundledFilePathAsNSString(const char *f) {
	return [NSString stringWithFormat:@"%@/%s", [[NSBundle mainBundle] resourcePath], f];
}

#endif
