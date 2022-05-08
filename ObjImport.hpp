#ifndef ObjImport_h
#define ObjImport_h

#include "CoordinateTypes.hpp"

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
	
	v3 *vertices;		// free() these when deleting
	v3 *normals;
	v2 *texcoords;
	unsigned int *elements;
} ObjFile;


ObjFile loadObjFile(const char *filename);
void printObjFileSummary(ObjFile *, const char *filename);

#endif
