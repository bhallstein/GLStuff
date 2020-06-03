//
//  Vectors.cpp
//  ComponentTest
//
//  Created by Ben on 16/11/2015.
//  Copyright © 2015 Ben. All rights reserved.
//

#include "Vectors.hpp"


float dotProduct(const v2 &a, const v2 &b) {
	return a.x * b.x + a.y * b.y;
}
float dotProduct(const v3 &a, const v3 &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

//float crossProduct(v3a , v3 b) {
//	
//}
//float crossProduct(Triangle) {
//	
//}


