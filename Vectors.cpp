//
//  Vectors.cpp
//  ComponentTest
//
//  Created by Ben on 16/11/2015.
//  Copyright © 2015 Ben. All rights reserved.
//

#include "Vectors.hpp"


float dotProduct(v2 a, v2 b) {
	return a.x * b.x + a.y * b.y;
}
float dotProduct(v3 a, v3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

//float crossProduct(v3, v3) {
//	
//}
//float crossProduct(Triangle) {
//	
//}


