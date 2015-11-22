#version 150

uniform sampler2D state;

uniform ivec2 size;

out vec4 xgrad;
out vec4 ygrad;

#define PI 3.1415926535897932384626433832795

vec2 invsize = 1./vec2(size);

//compute the color gradient by finite difference in the given direction

//use a sample function to handle different texture types+topologies
vec3 sample(in vec2 p){
	return texture(state, p*invsize).rgb;
}

vec3 grad(vec2 dir){
	vec2 p = gl_FragCoord.xy;
	vec2 d = dir/length(dir);
	vec3 above = sample(p+d);
	vec3 below = sample(p-d);
	vec3 ret = (above-below)/2.;//(length(above-below)+.1);//normalize(above-below);//.5*(above-below);
	return ret;
}

void main() {
    xgrad = vec4(grad(vec2(1.,0.)), 1.);
    ygrad = vec4(grad(vec2(0.,1.)), 1.);
}
