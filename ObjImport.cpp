#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#include "ObjImport.hpp"

// String & vector helper fns
// --------------------------------------

std::vector<std::string> strSplit(const std::string &s, char delim) {
	std::vector<std::string> v;
	std::stringstream ss(s);
	std::string chunk;
	while(std::getline(ss, chunk, delim)) {
		v.push_back(chunk);
	}
	return v;
}

void strTrim(std::string &s) {
	int i = int(s.size()) - 1;
	while (i >= 0 && (s[i] == ' ' || s[i] == '\t'))
		--i;
	s = s.substr(0, i+1);
}

std::vector<float> splitToFloatVec(const std::string &s, char delim = ' ') {
	std::vector<float> v;
	auto components = strSplit(s, delim);
	for (auto component : components) {
		v.push_back(std::atof(component.c_str()));
	}
	return v;
}
std::vector<int> splitToIntVec(const std::string &s, char delim = ' ') {
	std::vector<int> v;
	auto components = strSplit(s, delim);
	for (auto component : components) {
		v.push_back(std::atoi(component.c_str()));
	}
	return v;
}


// Struct types
// --------------------------------------

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


// ObjFile
// --------------------------------------

ObjFile ObjFile::load(const char *filename) {
	ObjFile o;

	std::ifstream file(filename);

//	std::vector<unsigned int> elements;
//	std::vector<v3> vertex_attrib_vec;
//	std::vector<v3> normal_attrib_vec;
//	std::vector<v2> texcoord_attrib_vec;

	std::map<Triple, int> triple_to_index_map;	// Store indices of our built attributes here

	std::vector<v3> v_store;
	std::vector<v3> n_store;
	std::vector<v2> t_store;

	// We assume the obj file specifies all "data" (v/vn/vt) before
	// embarking on specifying faces, so our "stores" will be
	// complete at that point.

	for (std::string line; std::getline(file, line); ) {
		o.nLines += 1;
		strTrim(line);

		// # designates a comment
		if (line[0] == '#') {
			o.nLinesComment += 1;
			continue;
		}

		// v designates a vertex
		else if (line[0] == 'v' && line[1] == ' ') {
			o.nLinesVertex += 1;

			std::vector<float> v = splitToFloatVec(line.substr(2, 1000));
			if (v.size() != 3) {
				printf("error on line %d: vertex had wrong number of coords (%lu rather than %d)\n",
							 o.nLines, v.size(), 3);
				printf("line:\n%s\n", line.c_str());
				break;
			}

			v_store.push_back((v3) { v[0], v[1], v[2] });
		}

		// vn designates a normal
		else if (line[0] == 'v' && line[1] == 'n') {
			o.nLinesNormal += 1;

			std::vector<float> v = splitToFloatVec(line.substr(3, 1000));
			if (v.size() != 3) {
				printf("error on line %d: normal had wrong number of coords (%lu rather than %d\n)\n",
							 o.nLines, v.size(), 3);
				printf("line:\n%s\n", line.c_str());
				break;
			}

			n_store.push_back((v3) { v[0], v[1], v[2] });
		}

		// vt designates a texcoord
		else if (line[0] == 'v' && line[1] == 't') {
			o.nLinesTexcoord += 1;

			std::vector<float> v = splitToFloatVec(line.substr(3, 1000));
			if (v.size() < 2) {
				printf("error on line %d: texcoord had wrong number of coords (%lu)\n",
							 o.nLines, v.size());
				printf("line:\n%s\n", line.c_str());
				break;
			}

			t_store.push_back((v2) { v[0], v[1] });
		}

		// f designates face
		else if (line[0] == 'f') {
			o.nLinesFace += 1;

			auto faceVerts = strSplit(line.substr(2, 1000), ' ');
			if (faceVerts.size() < 3) {
				printf("error on line %d: face has wrong number of components (%lu)\n",
							 o.nLines,
							 faceVerts.size());
				printf("line:\n%s\n", line.c_str());
				break;
			}

			for (int i=0; i < 3; ++i) {
				std::vector<int> v = splitToIntVec(faceVerts[i], '/');

				if (v.size() != 3) {
					printf("error on line %d: face component %d has wrong number of indices (%lu rather than %d)\n",
								 o.nLines, i+1, v.size(), 3);
					break;
				}

				Triple trpl(v[0]-1, v[1]-1, v[2]-1);

				// If we have an attrib index for this triple already, just push it to our vector of elements
				auto it = triple_to_index_map.find(trpl);
				if (it != triple_to_index_map.end()) {
					o.elements.push_back(it->second);
				}

				// If we don't, then create a new attrib entry (v, vn, vt), add its index to the elements vector,
				// and store it in the map
				else {
					int index_of_new_attrib = int(o.vertices.size());

					o.vertices.push_back(v_store[trpl.a]);
					o.texcoords.push_back(t_store[trpl.b]);
					o.normals.push_back(n_store[trpl.c]);
					o.elements.push_back(index_of_new_attrib);

					triple_to_index_map[trpl] = index_of_new_attrib;
				}
			}
		}

		else {
			o.nLinesEmpty += 1;
		}
	}

	if (o.vertices.size() != o.normals.size() || o.vertices.size() != o.texcoords.size()) {
		printf("  WARNING: attrib vec sizes differ! (v:%lu, n:%lu, t:%lu)\n",
					 o.vertices.size(),
					 o.normals.size(),
					 o.texcoords.size());
	}

	return o;
}

void ObjFile::printSummary(const char *filename) {
	printf("ObjFile '%s'':\n"
				 "  nLines:             %d  \n"
				 "  nLinesComment:      %d  \n"
				 "  nLinesVertex:       %d  \n"
				 "  nLinesNormal:       %d  \n"
				 "  nLinesTexcoord:     %d  \n"
				 "  nLinesFace:         %d  \n"
				 "  nLinesEmpty:        %d  \n"
				 "  vertices:           %lu \n"
				 "  elements:           %lu \n\n",
				 filename,
				 nLines,
				 nLinesComment,
				 nLinesVertex,
				 nLinesNormal,
				 nLinesTexcoord,
				 nLinesFace,
				 nLinesEmpty,
				 vertices.size(),
				 elements.size());
}
