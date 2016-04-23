#version 150

uniform sampler2D state;
uniform sampler2D modulation;
uniform ivec2 size;

out vec4 outputColor;

#define PI 3.1415926535897932384626433832795

vec2 invsize = 1./vec2(size);

vec4 sample(in vec2 p, sampler2D s){
	return texture(s, p*invsize);
}

void main() {

	vec2 p = gl_FragCoord.xy;

	vec2 d = sample(p, modulation).xy;

	vec4 c = sample(p+d, state);

	outputColor = c;
}
