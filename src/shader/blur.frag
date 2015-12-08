#version 150

uniform sampler2D state;
uniform vec2 dir;
uniform ivec2 size;

out vec4 outputColor;

#define PI 3.1415926535897932384626433832795

float r = length(dir);
vec2 d = dir/r;
float gauss_coeff = 2.1213/r;
vec2 invsize = 1./vec2(size);

//separable gaussian blur which wraps around the torus

//use a sample function to handle different texture types+topologies
vec3 sample(in vec2 p){
	return texture(state, p*invsize).rgb;
}

vec3 blur(){
	vec2 p = gl_FragCoord.xy;

	if(r<=0.)
		return sample(p);

	vec3 acc = vec3(0,0,0);
	float norm = 0.;

	int num = 2*int(ceil(r)); //number of hops == number of samples - 1
	float inc = 2.*r/(num);

	for(float i= -r; i<=r+.0001; i+=inc){
		float x = i*gauss_coeff;
		float weight = exp(-x*x);
		acc+= weight*sample(p+i*d);
		norm+= weight;
	}
	acc/=norm;
	return acc;
}

void main() {

    outputColor = vec4(blur(), 1.);
}
