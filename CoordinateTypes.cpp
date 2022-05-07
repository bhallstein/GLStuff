//
//  CoordinateTypes.cpp
//  UI
//
//  Created by Ben on 30/08/2016.
//  Copyright © 2016 Ben. All rights reserved.
//

#include "CoordinateTypes.h"

bool eq(const v2 &a, const v2 &b) {
	return a.x == b.x && a.y == b.y;
}
bool eq(const v2i &a, const v2i &b) {
	return a.x == b.x && a.y == b.y;
}
bool eq(const v3 &a, const v3 &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}
bool eq(const v3i &a, const v3i &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}
bool eq(const v4 &a, const v4 &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z && a.t == b.t;
}
bool eq(const col3 &a, const col3 &b) {
	return a.r == b.r && a.g == b.g && a.b == b.b;
}
bool eq(const col4 &a, const col4 &b) {
	return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}
