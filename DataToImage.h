//
//  DataToImage.h
//  ComponentTest
//
//  Created by Ben on 16/11/2015.
//  Copyright © 2015 Ben. All rights reserved.
//

#ifndef DataToImage_h
#define DataToImage_h

#ifdef __cplusplus
extern "C" {
#endif

void dataToImage_grayscale_unsigned_char(unsigned char *data, int width, int height, const char *filename);
void dataToImage_grayscale_float_0to1(float *, int width, int height, const char *filename);
	
#ifdef __cplusplus
}
#endif

#endif
