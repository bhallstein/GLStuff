#version 140

uniform mat4 vpMtx;
uniform mat3 modelMtx;
uniform mat3 normalMtx;

in vec3 inVPos;
in vec3 inVNormal;
in vec2 inTexCoord;

in vec4 inQuaternion;   // Rotation to apply to model (quaternion)
in vec3 inTranslation;	// Translation to apply after rotating

out vec3 normal_world;
out vec3 fragpos_world;
out vec2 outTexCoord;

/* Rotate using quaternion */
vec3 quat_rotate(vec3 v, vec4 q) {
	return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
}

void main(void) {
	vec3 p_world = inTranslation + quat_rotate(modelMtx * inVPos, inQuaternion);
	
	gl_Position = vpMtx * vec4(p_world, 1.0);

	normal_world = normalMtx * quat_rotate(inVNormal, inQuaternion);
	fragpos_world = p_world;
	
	outTexCoord = inTexCoord;
}

