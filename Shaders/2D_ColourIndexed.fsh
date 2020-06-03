#version 140

in vec3 outColour;
out vec4 fragOutColour;

uniform float uMix;

void main(void) {
	fragOutColour = vec4(outColour, 1.0);
}
