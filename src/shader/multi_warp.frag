#version 150

uniform sampler2D src;
uniform sampler2D disp0;
uniform sampler2D disp1;
uniform sampler2D disp2;
uniform sampler2D disp3;
uniform sampler2D disp4;
uniform sampler2D disp5;

uniform float t;
uniform ivec2 size;
uniform int num_tex;

out vec4 outputColor;

#define PI 3.1415926535897932384626433832795

vec2 invsize = 1./vec2(size);

float sigmoid(float x){
	return x/(1.+abs(x));
}
vec2 sigmoid(vec2 x){
	return x/(1.+abs(x));
}
vec3 sigmoid(vec3 x){
	return x/(1.+abs(x));
}

vec2 msigmoid(vec2 x){
	return x/(1.+length(x));
}
vec3 msigmoid(vec3 x){
	return x/(1.+length(x));
}

//convert between bipolar [-1, 1] and unipolar [0, 1]
vec3 u2b(vec3 u){
	return 2.*u-1.;
}
vec3 b2u(vec3 b){
	return .5*b+.5;
}

vec3 sample(in vec2 p, sampler2D s){
	return texture(s, p*invsize).rgb;
}

vec3 sample(in vec2 p, int t){
	switch(t){
		case 0:
			return sample(p, disp0);
		case 1:
			return sample(p, disp1);
		case 2:
			return sample(p, disp2);
		case 3:
			return sample(p, disp3);
		case 4:
			return sample(p, disp4);
		case 5:
			return sample(p, disp5);
	}
}

void main() {
	vec2 p = gl_FragCoord.xy;

	vec3 c = vec3(0.);

	for(int i=0; i<num_tex; i++){
		vec2 dp = sample(p, i).xy;
		c += sample(p+dp, src);
	}

	c/= float(num_tex);

    outputColor = vec4(c, 1.);
}