#ifndef ObjImport_h
#define ObjImport_h

#include <vector>
#include "CoordinateTypes.hpp"

typedef struct ObjFile {
	int nLines = 0;
	int nLinesComment = 0;
	int nLinesVertex = 0;
	int nLinesNormal = 0;
	int nLinesTexcoord = 0;
	int nLinesFace = 0;
	int nLinesEmpty = 0;
	
	std::vector<unsigned int> elements;
	std::vector<v3> vertices;
	std::vector<v3> normals;
	std::vector<v2> texcoords;

	static ObjFile load(const char *filename);
	void printSummary(const char *filename);
} ObjFile;

#endif
