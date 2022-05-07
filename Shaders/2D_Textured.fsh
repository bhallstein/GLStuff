#version 140

in vec2 outTexCoord;
out vec4 fragOutColour;

uniform sampler2D uTex;

void main(void) {
	fragOutColour = texture(uTex, outTexCoord.st, 0.0);
}

