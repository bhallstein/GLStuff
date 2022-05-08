#import <Foundation/Foundation.h>
#include "FilePaths_CPP.hpp"

char* bundledFilePath(const char *f) {
	NSString *path = [NSString stringWithFormat:@"%@/%s", [[NSBundle mainBundle] resourcePath], f];
	return strdup([path UTF8String]);
}
