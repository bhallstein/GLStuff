#version 140

uniform vec3 uLightVector;			// Light dir. from inf. dist
uniform vec3 uLightProperties;		// Ambient, diffuse, specular

uniform vec3 uCamPosition;

uniform vec3 uColour;
uniform float uColToLightRatio;

in vec3 normal_world;
in vec3 fragpos_world;
out vec4 fragOutColour;
out vec3 fragOutNormal;

void main(void) {

	float l_Ambient = uLightProperties[0];
	float l_Diffuse = uLightProperties[1];
	float l_Specular = uLightProperties[2];

	// Renormalize fragment normal as it has been interpolated
	vec3 normal = normalize(normal_world);
	fragOutNormal = normal;
	
	fragOutColour = uColour;
	
//	// 1. Phong lighting
//
//	// Diffuse is proportional to l.n
//	float diff = l_Diffuse * max(0.0, dot(-uLightVector, normal));
//
//	// Specular is proportional to r.v ^ exp
//	vec3 v = normalize(fragpos_world - uCamPosition);
//	vec3 r = reflect(-uLightVector, normal);
//	float exp = 7.0;
//	float spec = 0.7 * l_Specular * pow(max(dot(r,v),0), exp);
//	
//	
//	// 2. Combine
//
//	float light_combined = l_Ambient + diff + spec;
//	fragOutColour = vec4(uColour, 1.0) * (light_combined + 0.1) + uColToLightRatio - uColToLightRatio;
}

