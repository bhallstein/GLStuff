#version 140

in vec4 outColour;
out vec4 fragOutColour;

uniform sampler2D uTex;

void main(void) {
	float d = texture(uTex, gl_FragCoord.xy * 0.125, 0).r;
	d = d * 0.03125 - 0.0078125*2.0;
	
	fragOutColour = outColour + vec4(d);
}

// Dithering: see http://www.anisopteragames.com/how-to-fix-color-banding-with-dithering/
//                http://loopit.dk/banding_in_games.pdf
