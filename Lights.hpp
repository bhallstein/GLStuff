#ifndef Lights_h
#define Lights_h

#include "vectors.hpp"

struct DirectionalLight {
	v3 lightVector;
	v3 lightProperties;  // amb, diff, spec
};

// A directional light has parallel rays

// A point light is a point, radiating light in a sphere
// - so reflection angles depend on the fragment's location
// - also attenuates

// A spot light is similar to a point, but has direction, and
// a radius for attenuation

#endif
