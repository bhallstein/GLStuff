#version 140

in vec2 inVPos;
in vec4 inColour;

uniform mat4 ortho_matrix;

out vec4 outColour;

void main(void) {
	gl_Position = ortho_matrix * vec4(inVPos, 1.0, 1.0);
	outColour = inColour;
}
