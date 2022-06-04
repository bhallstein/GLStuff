#import <Foundation/Foundation.h>
#import <ImageIO/ImageIO.h>
#include "TexImage.hpp"
#include "GLHelpers.hpp"

struct TexImage null_tex_image = { NULL, -1, -1 };

struct TexImage loadPngTexture(const char *filepath) {
	CFURLRef texURL = (CFURLRef) CFBridgingRetain([NSURL fileURLWithPath:[NSString stringWithFormat:@"%s", filepath]]);
	if (!texURL) {
		NSLog(@"TexImage: Texture name invalid -- %s", filepath);
		return null_tex_image;
	}
	
	CGImageSourceRef imageSource = CGImageSourceCreateWithURL(texURL, NULL);
	if (!imageSource) {
		NSLog(@"TexImage: Invalid image path %s", filepath);
		CFRelease(texURL);
		return null_tex_image;
	}
	if (CGImageSourceGetCount(imageSource) <= 0) {
		NSLog(@"TexImage: No image in image source");
		CFRelease(texURL);
		return null_tex_image;
	}
	CFRelease(texURL);
	
	CGImageRef image = CGImageSourceCreateImageAtIndex(imageSource, 0, NULL);
	if (!image)	{
		NSLog(@"Image not created");
		return null_tex_image;
	}
	CFRelease(imageSource);
	
	GLuint width = (GLuint) CGImageGetWidth(image);
	GLuint height = (GLuint) CGImageGetHeight(image);
	
	void *data = malloc(width * height * 4);
	
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	if (!colorSpace) {
		NSLog(@"TexImage: Colorspace not created");
		free(data);
		return null_tex_image;
	}
	
	CGContextRef context = CGBitmapContextCreate(data,
												 width,
												 height,
												 8,
												 width * 4,
												 colorSpace,
												 kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);
	if (!context) {
		NSLog(@"TexImage: Context not created");
		free(data);
		return null_tex_image;
	}
	
	CGColorSpaceRelease(colorSpace);
	
	// Flip so that it isn't upside-down
	CGContextTranslateCTM(context, 0, height);
	CGContextScaleCTM(context, 1.0f, -1.0f);
	CGContextSetBlendMode(context, kCGBlendModeCopy);
	CGContextDrawImage(context, CGRectMake(0, 0, width, height), image);
	CGImageRelease(image);
	CGContextRelease(context);
	
	return (struct TexImage) { data, float(width), float(height) };
}


int loadCubeMap(unsigned int tex_id, const char *fnames[]) {
	glActiveTexture(GL_TEXTURE0);
	Texture::bind_cube_map(0, tex_id);
	for (int i=0; i < 6; ++i) {
		struct TexImage texImage = loadPngTexture(fnames[i]);
		if (texImage.data == NULL) {
			return false;
		}
		Texture::CubeMapFace face = (Texture::CubeMapFace) (Texture::CubeMapRight + i);
		Texture::upload_cube_map_face(texImage.w, texImage.h, texImage.data, face);
		free(texImage.data);
	}
	return true;
}
