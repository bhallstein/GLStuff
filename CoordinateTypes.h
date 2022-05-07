//
//  CoordinateTypes.h
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#ifndef __COORDINATE_TYPES_H
#define __COORDINATE_TYPES_H

struct v2  {  float x, y;  };
struct v2i {  int x, y;    };


struct v3  {  float x, y, z;  };
struct v3i {  int x, y, z;    };

struct v4  {  float x, y, z, t;  };

struct col3 {  float r, g, b;  };
struct col4 {  float r, g, b, a; };


#ifdef __cplusplus
bool eq(const v2   &, const v2   &);
bool eq(const v2i  &, const v2i  &);
bool eq(const v3   &, const v3   &);
bool eq(const v3i  &, const v3i  &);
bool eq(const v4   &, const v4   &);
bool eq(const col3 &, const col3 &);
bool eq(const col4 &, const col4 &);
#endif

#endif

