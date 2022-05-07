#version 140

in vec4 outColour;
out vec4 fragOutColour;

uniform vec4 uOuterColour;
uniform vec4 uInnerColour;

void main(void) {
	fragOutColour = outColour;
}
