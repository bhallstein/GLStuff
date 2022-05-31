#include "TextTex.hpp"
#include <Cocoa/Cocoa.h>

typedef NSBitmapImageRep ImRep;

struct TexImage get_textTex(const char *_str,
                            float fitWidth,
                            float fitHeight,
                            float font_size,
                            v4 col,
                            v4 bg) {
	NSFont *f = [NSFont fontWithName:@"Helvetica" size:font_size];
	
	// Create NSImage
	NSString *str = [NSString stringWithUTF8String:_str];
	NSImage *im = [[NSImage alloc] initWithSize:NSMakeSize(fitWidth, fitHeight)];
	ImRep *imrep = [[ImRep alloc] initWithBitmapDataPlanes:NULL
												pixelsWide:fitWidth
												pixelsHigh:fitHeight
											 bitsPerSample:8
										   samplesPerPixel:4
												  hasAlpha:YES
												  isPlanar:NO
											colorSpaceName:NSCalibratedRGBColorSpace
											   bytesPerRow:0
											  bitsPerPixel:0];
	[im addRepresentation:imrep];
	[im lockFocus];
	[str drawInRect:NSMakeRect(0, 0, fitWidth, fitHeight)
	 withAttributes:@{
					  NSFontAttributeName : f,
					  NSForegroundColorAttributeName : [NSColor colorWithRed:col.r green:col.g blue:col.b alpha:col.a],
					  NSBackgroundColorAttributeName : [NSColor colorWithRed:bg.r green:bg.g blue:bg.b alpha:bg.a],
					  }];
	[im unlockFocus];
	
	
	// Draw into CGContext to get pixel data
	void *data = malloc(fitWidth * fitHeight * 4);
	NSRect imageRect = NSMakeRect(0, 0, im.size.width, im.size.height);
	CGImageRef cgim = [im CGImageForProposedRect:&imageRect context:NULL hints:nil];
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	if (!colorSpace) {
		NSLog(@"TexImage: Colorspace not created");
		free(data);
		return null_tex_image;
	}
	CGContextRef context = CGBitmapContextCreate(data,
												 fitWidth,
												 fitHeight,
												 8,
												 fitWidth * 4,
												 colorSpace,
												 kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);
	if (!context) {
		NSLog(@"TexImage: Context not created");
		free(data);
		return null_tex_image;
	}
	
	CGColorSpaceRelease(colorSpace);
	
	// Flip so that it isn't upside-down
	CGContextTranslateCTM(context, 0, fitHeight);
	CGContextScaleCTM(context, 1.0f, -1.0f);
	CGContextSetBlendMode(context, kCGBlendModeCopy);
	CGContextDrawImage(context, CGRectMake(0, 0, fitWidth, fitHeight), cgim);
	CGContextRelease(context);
	
	return (struct TexImage) { data, fitWidth, fitHeight };
}

