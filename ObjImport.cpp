#include "ObjImport.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>

#pragma mark String & vector helper fns

void strSplit(const std::string &s, std::vector<std::string> &v, char delim) {
	v.clear();
	std::stringstream ss(s);
	std::string chunk;
	while(std::getline(ss, chunk, delim))
		v.push_back(chunk);
}

void strTrim(std::string &s) {
	int i = int(s.size()) - 1;
	while (i >= 0 && (s[i] == ' ' || s[i] == '\t'))
		--i;
	s = s.substr(0, i+1);
}

void splitToFloatVec(const std::string &s, std::vector<float> &v, char delim = ' ') {
	std::vector<std::string> components_as_strings;
	strSplit(s, components_as_strings, delim);
	for (int i=0; i < components_as_strings.size(); ++i)
		v.push_back(std::atof(components_as_strings[i].c_str()));
}
void splitToIntVec(const std::string &s, std::vector<int> &v, char delim = ' ') {
	std::vector<std::string> components_as_strings;
	strSplit(s, components_as_strings, delim);
	for (int i=0; i < components_as_strings.size(); ++i)
		v.push_back(std::atoi(components_as_strings[i].c_str()));
}


#pragma mark - Struct types

struct Triple {
	Triple(int _a, int _b, int _c) : a(_a), b(_b), c(_c) { }
	unsigned int a, b, c;
	bool operator< (const Triple& t) const {            // Will cope with values up to 2^20 (about 1 million)
		unsigned long x1 = a,   y1 = b,   z1 = c;       // in each field
		unsigned long x2 = t.a, y2 = t.b, z2 = t.c;
		x1 <<= 40;
		x2 <<= 40;
		y1 <<= 20;
		y2 <<= 20;
		return (x1+y1+z1 < x2+y2+z2);
	}
	bool operator== (const Triple& t) {
		return (a == t.a && b == t.b && c == t.c);
	}
};
	

#pragma mark - Parsing function

ObjFile loadObjFile(const char *filename) {
	
	ObjFile objFile = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL
	};
	
	std::ifstream file(filename);
	
	std::vector<unsigned int> elements;
	std::vector<v3> vertex_attrib_vec;
	std::vector<v3> normal_attrib_vec;
	std::vector<v2> texcoord_attrib_vec;
	
	std::map<Triple, int> triple_to_index_map;	// Store indices of our built attributes here
	
	std::vector<v3> v_store, n_store;
	std::vector<v2> t_store;				// We assume the obj file specifies all "data" (v/vn/vt) before
											// embarking on specifying faces, so our "stores" will be
											// complete at that point.
	
	for (std::string line; std::getline(file, line); ) {
		++objFile.nLines;
		
		strTrim(line);
		
		// # designates a comment
		if (line[0] == '#') {
			++objFile.nLinesComment;
			continue;
		}
		
		else if (line[0] == 'v') {
			
			// v designates a vertex
			if (line[1] == ' ') {
				// process vertex
				++objFile.nLinesVertex;
				
				std::vector<float> v;
				splitToFloatVec(line.substr(2, 1000), v);
				
				if (v.size() != 3) {
					printf("error on line %d: vertex had wrong number of coords (%lu rather than %d)\n",
						   objFile.nLines, v.size(), 3);
					printf("line:\n%s\n", line.c_str());
					break;
				}
				
				v_store.push_back((v3) { v[0], v[1], v[2] });
			}
			
			// vn designates a normal
			else if (line[1] == 'n') {
				// process normal
				++objFile.nLinesNormal;
				
				std::vector<float> v;
				splitToFloatVec(line.substr(3, 1000), v);
				
				if (v.size() != 3) {
					printf("error on line %d: normal had wrong number of coords (%lu rather than %d\n)\n",
						   objFile.nLines, v.size(), 3);
					printf("line:\n%s\n", line.c_str());
					break;
				}
				
				n_store.push_back((v3) { v[0], v[1], v[2] });
			}
			
			// vt designates a texcoord
			else if (line[1] == 't') {
				// process texcoord
				++objFile.nLinesTexcoord;
				
				std::vector<float> v;
				splitToFloatVec(line.substr(3, 1000), v);
				
				if (v.size() < 2) {
					printf("error on line %d: texcoord had wrong number of coords (%lu)\n",
						   objFile.nLines, v.size());
					printf("line:\n%s\n", line.c_str());
					break;
				}
				
				t_store.push_back((v2) { v[0], v[1] });
			}
			
		}
		
		else if (line[0] == 'f') {
			// Process face
			++objFile.nLinesFace;
			
			std::vector<std::string> faceVerts;
			strSplit(line.substr(2, 1000), faceVerts, ' ');
			if (faceVerts.size() < 3) {
				printf("error on line %d: face has wrong number of components (%lu)\n",
					   objFile.nLines, faceVerts.size());
				printf("line:\n%s\n", line.c_str());
				break;
			}
			
			for (int i=0; i < 3; ++i) {
				std::vector<int> v;
				splitToIntVec(faceVerts[i], v, '/');
				
				if (v.size() != 3) {
					printf("error on line %d: face component %d has wrong number of indices (%lu rather than %d)\n",
						   objFile.nLines, i+1, v.size(), 3);
					break;
				}
				
				Triple trpl(v[0]-1, v[1]-1, v[2]-1);
				
				// If we have an attrib index for this triple already, just push it to our vector of elements
				std::map<Triple, int>::iterator it = triple_to_index_map.find(trpl);
				if (it != triple_to_index_map.end()) {
					elements.push_back(it->second);
				}
				
				// If we don't, then create a new attrib entry (v, vn, vt), add its index to the elements vector,
				// and store it in the map
				else {
					int index_of_new_attrib = int(vertex_attrib_vec.size());
					
					vertex_attrib_vec.push_back(v_store[trpl.a]);
					texcoord_attrib_vec.push_back(t_store[trpl.b]);
					normal_attrib_vec.push_back(n_store[trpl.c]);
					
					elements.push_back(index_of_new_attrib);
					
					triple_to_index_map[trpl] = index_of_new_attrib;
				}
			}
		}
		
		else {
			// do nothing
			++objFile.nLinesEmpty;
		}
	}
	
	if (vertex_attrib_vec.size() != normal_attrib_vec.size() || vertex_attrib_vec.size() != texcoord_attrib_vec.size())
		printf("  WARNING: attrib vec sizes differ! (v:%lu, n:%lu, t:%lu)\n",
			   vertex_attrib_vec.size(),
			   normal_attrib_vec.size(),
			   texcoord_attrib_vec.size()
			   );
	
	// Copy vectors into C-style arrays in ObjFile
	objFile.nVerticesImported = (int) vertex_attrib_vec.size();
	objFile.nElements = (int) elements.size();
	
	objFile.vertices  = (v3*) malloc(sizeof(v3) * objFile.nVerticesImported);
	objFile.normals   = (v3*) malloc(sizeof(v3) * objFile.nVerticesImported);
	objFile.texcoords = (v2*) malloc(sizeof(v2) * objFile.nVerticesImported);
	for (int i=0; i < objFile.nVerticesImported; ++i) {
		objFile.vertices[i]  = vertex_attrib_vec[i];
		objFile.normals[i]   = normal_attrib_vec[i];
		objFile.texcoords[i] = texcoord_attrib_vec[i];
	}
	objFile.elements = (unsigned int*) malloc(sizeof(unsigned int) * objFile.nElements);
	for (int i=0; i < objFile.nElements; ++i)
		objFile.elements[i] = elements[i];
	
	return objFile;
}

void printObjFileSummary(ObjFile *f, const char *filename) {
	printf(
		"ObjFile '%s'':\n"
		"  nLines:             %d  \n"
		"  nLinesComment:      %d  \n"
		"  nLinesVertex:       %d  \n"
		"  nLinesNormal:       %d  \n"
		"  nLinesTexcoord:     %d  \n"
		"  nLinesFace:         %d  \n"
		"  nLinesEmpty:        %d  \n"
		"  nVerticesImported:  %d  \n"
		"  nElements:          %d  \n\n",
		filename,
		f->nLines,
		f->nLinesComment,
    	f->nLinesVertex,
    	f->nLinesNormal,
    	f->nLinesTexcoord,
    	f->nLinesFace,
    	f->nLinesEmpty,
    	f->nVerticesImported,
    	f->nElements
	);
}

