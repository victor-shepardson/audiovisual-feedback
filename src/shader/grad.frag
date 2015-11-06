#version 150

uniform sampler2D state;
uniform vec2 dir;
uniform ivec2 size;

out vec4 outputColor;

#define PI 3.1415926535897932384626433832795

vec2 invsize = 1./vec2(size);

//compute the color gradient by finite difference in the given direction

//use a sample function to handle different texture types+topologies
vec3 sample(in vec2 p){
	return texture(state, p*invsize).rgb;
}

vec3 grad(){
	vec2 p = gl_FragCoord.xy;
	vec2 d = dir/length(dir);
	return .5*(sample(p+d) - sample(p-d));
}

void main() {
    outputColor = vec4(grad(), 1.);
}
