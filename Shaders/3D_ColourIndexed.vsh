#version 140

uniform mat4 mvpMtx;
uniform mat4 modelMtx;
uniform mat3 normalMtx;

in vec3 inVPos;
in vec3 inVNormal;
in vec3 inColour;

out vec3 normal_world;
out vec3 fragpos_world;
out vec3 outColour;

void main(void) {
	vec4 p = vec4(inVPos, 1.0);
	
	gl_Position = mvpMtx * p;
	
	normal_world = normalMtx * inVNormal;
	fragpos_world = vec3(modelMtx * p);
	outColour = inColour;
}

