#version 140

uniform mat4 mvpMtx;
uniform mat4 modelMtx;
uniform mat3 normalMtx;

in vec3 inVPos;
in vec3 inVNormal;
in vec2 inTexCoord;

out vec3 normal_world;
out vec3 fragpos_world;
out vec2 outTexCoord;

void main(void) {
	gl_Position = mvpMtx * vec4(inVPos, 1.0);

	normal_world = normalMtx * inVNormal;
	fragpos_world = vec3(modelMtx * vec4(inVPos, 1.0));
	outTexCoord = inTexCoord;
}

