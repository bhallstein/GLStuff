#version 140

// Light uniforms
uniform vec3 uLightVector;
	// Our light points in this direction, from an infinite
	// distance away
uniform vec3 uLightProperties;
	// (ambient, diffuse, specular)

uniform vec3 uCamPosition;

uniform sampler2D uTexture;
uniform samplerCube uCubeMapTex;
uniform float texToLightRatio;

in vec3 normal_world, fragpos_world;
in vec2 outTexCoord;
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


	// 2. Texturing

	vec4 texCol = texture(uTexture, outTexCoord, 0.0);


	// 3. Combine the two

	fragOutColour =
		texToLightRatio * texCol +
		(1.0 - texToLightRatio) * vec4(
			l_Ambient + diff + spec,
			l_Ambient + diff + spec,
			l_Ambient + diff + spec,
			1.0
		);
}
