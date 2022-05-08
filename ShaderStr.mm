#include "ShaderStr.hpp"
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
