//
//  DataToImage.m
//  ComponentTest
//
//  Created by Ben on 16/11/2015.
//  Copyright © 2015 Ben. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

void dataToImage_grayscale_unsigned_char(unsigned char *data, int width, int height, const char *fn) {
	NSBitmapImageRep *rep = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes: nil  // allocate the pixel buffer for us
																	pixelsWide: width
																	pixelsHigh: height
																 bitsPerSample: 8
															   samplesPerPixel: 1
																	  hasAlpha: NO
																	  isPlanar: NO
																colorSpaceName: NSCalibratedWhiteColorSpace // 0 = black, 1 = white in this color space
																   bytesPerRow: 0     // passing 0 means "you figure it out"
																  bitsPerPixel: 8];   // this must agree with bitsPerSample and samplesPerPixel
	
	NSInteger bytes_per_row = [rep bytesPerRow];
	
	unsigned char *pixels = [rep bitmapData];
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
			pixels[bytes_per_row * j + i] = (unsigned char) data[j*width + i];
	
	[[rep TIFFRepresentation] writeToFile:[NSString stringWithUTF8String:fn] atomically:NO];
}

void dataToImage_grayscale_float_0to1(float *data, int width, int height, const char *filename) {
	unsigned char *data_ch = (unsigned char *) malloc(width * height);
	for (int i=0; i < width; ++i)
		for (int j=0; j < height; ++j)
			data_ch[j*width + i] = 255 * data[j*width + i];
	
	dataToImage_grayscale_unsigned_char(data_ch, width, height, filename);
	free(data_ch);
}

