#version 150

uniform sampler2D state;
uniform sampler2D derivative;

uniform ivec2 size;
uniform float dt;

out vec4 outputColor;

vec2 invsize = 1./vec2(size);

//use a sample function to handle different texture types+topologies
vec3 sample(in vec2 p, sampler2D s){
	return texture(s, p*invsize).rgb;
}

void main() {
	vec2 p = gl_FragCoord.xy;

	vec3 c = sample(p, state);

	c += dt*sample(p, derivative);

    outputColor = vec4(c, 1.);
}
