#version 140

in vec2 inVPos;
in vec3 inColour;

out vec3 outColour;

void main(void) {
	gl_Position = vec4(inVPos, 1.0, 1.0);
	outColour = inColour;
}
