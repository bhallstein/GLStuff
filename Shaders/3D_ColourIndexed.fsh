#version 140

uniform vec3 uLightVector;			// Light dir. from inf. dist
uniform vec3 uLightProperties;		// Ambient, diffuse, specular

uniform vec3 uCamPosition;
uniform float uColToLightRatio;

in vec3 normal_world, fragpos_world;
in vec3 outColour;
out vec4 fragOutColour;

void main(void) {

	float l_Ambient = uLightProperties[0];
	float l_Diffuse = uLightProperties[1];
	float l_Specular = uLightProperties[2];

	// Renormalize fragment normal as it has been interpolated
	vec3 normal = normalize(normal_world);


	// 1. Phong lighting

	// Diffuse is proportional to l.n
	float diff = l_Diffuse * max(0.0, dot(-uLightVector, normal));

	// Specular is proportional to r.v ^ exp
	vec3 v = normalize(fragpos_world - uCamPosition);
	vec3 r = reflect(-uLightVector, normal);
	float exp = 7.0;
	float spec = 0.7 * l_Specular * pow(max(dot(r,v),0), exp);


	// 2. Colour indexing

	vec4 indexedColour = vec4(outColour, 1.0);


	// 3. Combine
	float light_combined = l_Ambient + diff + spec;
	fragOutColour = indexedColour * (light_combined + 0.1) + uColToLightRatio - uColToLightRatio;
}
