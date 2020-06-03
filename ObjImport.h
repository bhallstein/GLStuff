//
//  ObjImport.h
//  OpenGL3.2-ObjParsing
//
//  Created by Ben on 14/10/2013.
//  Copyright (c) 2013 Ben. All rights reserved.
//

#ifndef __OBJ_IMPORT_H
#define __OBJ_IMPORT_H

#include "CoordinateTypes.h"

typedef struct ObjFile {
	int nLines,
		nLinesComment,
		nLinesVertex,
		nLinesNormal,
		nLinesTexcoord,
		nLinesFace,
		nLinesEmpty;
	
	int nVerticesImported,
		nElements;
	
	v3f *vertices;		// free() these when deleting
	v3f *normals;
	v2f *texcoords;
	unsigned int *elements;
} ObjFile;


ObjFile loadObjFile(const char *filename);
void printObjFileSummary(ObjFile *, const char *filename);

#endif
