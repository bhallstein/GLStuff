//
//  TexImage.c
//  OpenGL3.2-Mac
//
//  Created by Ben on 23/09/2013.
//  Copyright (c) 2013 Ben. All rights reserved.
//

#include "TexImage.h"
//#include <stdio.h>
#import <Foundation/Foundation.h>
#import <OpenGL/gl.h>


TexImage loadPngTexture(const char *filepath) {
	
//    CFURLRef textureURL = CFBundleCopyResourceURL(CFBundleGetMainBundle(),
//												  (__bridge CFStringRef)fileName,
//												  CFSTR("png"),
//												  NULL);
	CFURLRef texURL = CFBridgingRetain([NSURL fileURLWithPath:[NSString stringWithFormat:@"%s", filepath]]);
	if (!texURL) {
		NSLog(@"Texture name invalid");
		return (TexImage) { NULL, -1, -1 };
	}
	
	CGImageSourceRef imageSource = CGImageSourceCreateWithURL(texURL, NULL);
	if (!imageSource) NSLog(@"Invalid image path");
	if (CGImageSourceGetCount(imageSource) <= 0)
		NSLog(@"No image in image source");
	CFRelease(texURL);
	
	CGImageRef image = CGImageSourceCreateImageAtIndex(imageSource, 0, NULL);
	if (!image)	NSLog(@"Image not created");
	CFRelease(imageSource);
	
	GLuint width = (GLuint) CGImageGetWidth(image);
	GLuint height = (GLuint) CGImageGetHeight(image);
	
	void *data = malloc(width * height * 4);
	
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	if (!colorSpace) NSLog(@"Colorspace not created");
	
	CGContextRef context = CGBitmapContextCreate(data,
												 width,
												 height,
												 8,
												 width * 4,
												 colorSpace,
												 kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);
	if (!context) NSLog(@"Context not created");
	
	CGColorSpaceRelease(colorSpace);

	// Flip so that it isn't upside-down
	CGContextTranslateCTM(context, 0, height);
	CGContextScaleCTM(context, 1.0f, -1.0f);
	CGContextSetBlendMode(context, kCGBlendModeCopy);
	CGContextDrawImage(context, CGRectMake(0, 0, width, height), image);
	CGImageRelease(image);
	CGContextRelease(context);
	
    return (TexImage){ data, width, height };
}
