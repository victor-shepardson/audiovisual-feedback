#version 150

uniform sampler2D state;
uniform sampler2D disp;
uniform sampler2D lp;
uniform sampler2D rcv;

uniform float t;
uniform ivec2 size;

uniform float compress;
uniform float lp_gain;
uniform float rcv_gain;
uniform float saturate;
uniform float bias;
uniform float time_scale;
uniform float rot;
uniform float bound_clip;

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

//use a sample function to handle different texture types+topologies
vec3 sample(in vec2 p, sampler2D s){
	return texture(s, p*invsize).rgb;
}

float sgn(float x){
	if(x>0.) return 1.;
	else if(x<0.) return -1.;
	return 0.;
}

void main() {
	vec2 p = gl_FragCoord.xy;

	vec3 val_lp = sample(p, lp);
	vec3 val_state = sample(p, state);
	vec2 disp = sample(p, disp).xy;
	vec3 val_warped = sample(p+disp, state);	
	vec3 val_rcv = sigmoid(sample(p+disp, rcv));

	float mean = dot(val_warped, vec3(1./3));
	vec3 centered = val_warped - mean;

	vec3 sat = saturate*centered/(length(centered)+.001);

	//approach the new color
	vec3 d = (val_warped - val_state) + (lp_gain*val_lp) + sat + bias + abs(rcv_gain)*(sgn(rcv_gain)*val_rcv - val_state);

	d = mix(d, d/(length(d)+.001), compress);

	//some kind of color rotation
	d = mix(d, .5*sign(rot)*(d.gbr - d.brg), abs(rot));

	d *= time_scale;

	//force derivative to zero at bounds
	if(dot(d, val_state) > 0 && length(val_state) > bound_clip)
		d /= 2.*(length(val_state)-bound_clip)+1.;
	
    outputColor = vec4(d, 1.);
}