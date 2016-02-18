#version 150

uniform sampler2D x;
uniform sampler2D y;

uniform float t;
uniform ivec2 size;

uniform float alpha;
uniform float beta;
uniform float epsilon;
uniform float time_scale;

out vec4 outputColor;

#define PI 3.1415926535897932384626433832795

vec2 invsize = 1./vec2(size);

//use a sample function to handle different texture types+topologies
vec3 sample(in vec2 p, sampler2D s){
	return texture(s, p*invsize).rgb;
}


void main() {
	vec2 p = gl_FragCoord.xy;

	vec3 val_y = sample(p,y);
	vec3 val_x = sample(p,x);

	vec3 time_filter = alpha*.5*(val_x-val_y);

	vec3 diffusion = beta*.125*(-4.*val_y
		+ sample(p+vec2(0,1), y)
		+ sample(p+vec2(0,-1), y)
		+ sample(p+vec2(1,0), y)
		+ sample(p+vec2(-1,0), y));

	vec3 damp = -epsilon*val_y;

	vec3 d = time_scale * (time_filter + diffusion + damp);

    outputColor = vec4(d, 1.);
}