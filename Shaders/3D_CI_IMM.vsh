#version 140

uniform mat4 modelMtx;	// Only to be used for scaling etc,
uniform mat4 viewMtx;   // since reconstructing indiv. rotations / translations
uniform mat4 projMtx;
uniform mat3 normalMtx;

in vec3 inVPos;
in vec3 inVNormal;
in vec3 inColour;

in vec4 inModelRot;   // Rotation to apply (first) to model (quaternion)
in vec3 inModelPos;	  // Translation to apply (second)

out vec3 normal_world;
out vec3 fragpos_world;
out vec3 outColour;

/* Rotate using quaternion */
vec3 quat_rotate(vec3 position, vec4 q) {
	vec3 v = position.xyz;
	return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
}

void main(void) {
	mat4 translMtx = mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		inModelPos.x, inModelPos.y, inModelPos.z, 1.0
	);
	
	vec4 p2 = vec4(quat_rotate(inVPos, inModelRot), 1.0);
	gl_Position = projMtx * viewMtx * translMtx * modelMtx * p2;
	
	normal_world = normalMtx * quat_rotate(inVNormal, inModelRot);
	fragpos_world = vec3(modelMtx * p2);
	
	outColour = inColour;
}

