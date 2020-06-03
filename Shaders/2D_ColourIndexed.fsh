#version 140

in vec3 outColour;
out vec4 fragOutColour;

void main(void) {
	fragOutColour = vec4(outColour, 1.0);
}
