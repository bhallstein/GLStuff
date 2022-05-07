#version 140

// This fragment shader expects a single-channel texture, such as a
// depth texture, and copies the value to r/g/b channels

in vec2 outTexCoord;
out vec4 fragOutColour;

uniform sampler2D uTex;

void main(void) {
	float x = 1.0 - texture(uTex, outTexCoord.st, 0.0).r;
	x *= 76.0;
	fragOutColour = vec4(x, x, x, 1.0);
}

