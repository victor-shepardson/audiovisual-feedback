#version 150

uniform sampler2D x;

uniform float t;
uniform ivec2 size;

out vec4 outputColor;

#define PI 3.1415926535897932384626433832795

vec2 invsize = 1./vec2(size);

vec4 sample(in vec2 p, sampler2D s){
	return texture(s, p*invsize);
}

void main() {
	vec2 p = gl_FragCoord.xy;
    outputColor = .5+.5*sample(p, x);
}