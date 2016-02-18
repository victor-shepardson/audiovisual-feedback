#version 150

uniform sampler2D src;
uniform sampler2D disp;

uniform float t;
uniform ivec2 size;


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

void main() {
	vec2 p = gl_FragCoord.xy;

	vec2 dp = sample(p, disp).xy;
	
	vec3 c = sample(p+dp, src);

    outputColor = vec4(c, 1.);
}