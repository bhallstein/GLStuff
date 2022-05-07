#version 140

in vec2 inVPos;
in vec2 inTexCoord;

uniform mat4 ortho_matrix;

out vec2 outTexCoord;

void main(void) {
	gl_Position = ortho_matrix * vec4(inVPos, 1.0, 1.0);
	outTexCoord = inTexCoord;
}

