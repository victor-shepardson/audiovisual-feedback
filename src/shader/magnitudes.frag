#version 150

uniform sampler2D xsrc;
uniform sampler2D ysrc;

uniform ivec2 size;

out vec4 mag;

#define PI 3.1415926535897932384626433832795

vec2 invsize = 1./vec2(size);

//compute the channel-wise squared magnitude of 2-vectors given by xsrc,ysrc

//use a sample function to handle different texture types+topologies
vec3 sample(in vec2 p, sampler2D x){
	return texture(x, p*invsize).rgb;
}

void main() {
   	vec2 p = gl_FragCoord.xy;
	vec3 x = sample(p, xsrc);
	vec3 y = sample(p, ysrc);
	mag = vec4(x*x+y*y, 1.);
}
