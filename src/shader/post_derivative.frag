#version 150

uniform sampler2D y;
uniform sampler2D yprime;
uniform sampler2D agents;

uniform float t;
uniform ivec2 size;

uniform	float target_sat;
uniform	float target_mean;
uniform float target_mix;
uniform float num_scales;
uniform	float time_scale;
uniform	float rot;
//uniform	float bound_gauss;
uniform	float bound_clip;
//uniform float warp;
//uniform float zoom;

//uniform mat4 color_proj;
//uniform mat4 grad_proj;

out vec4 outputColor;

#define PI 3.1415926535897932384626433832795

vec2 invsize = 1./vec2(size);

float sigmoid(float x){
	return x/(1.+abs(x));
}
vec3 sigmoid(vec3 x){
	return x/(1.+abs(x));
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

void main() {
	vec2 p = gl_FragCoord.xy;
	vec3 val_y = sample(p,y);
	vec3 val_new = sigmoid(sample(p,yprime));//(2.-pow(2.,1-num_scales));
	vec3 val_agents = sin(2.*PI*sigmoid(sample(p,agents)/(2.*PI)));//sigmoid(sample(p, agents));

	/*vec3 val_local = .25*(sample(p+vec2(1.,0.), yprime)
					+sample(p+vec2(-1.,0.), yprime)
					+sample(p+vec2(0.,1.), yprime)
					+sample(p+vec2(0.,-1.), yprime));
	val_new += val_new - val_local;
	*/
/*
	float d_mean = dot(d, vec3(1./3));
	vec3 d_centered = d - d_mean;

	float y_mean = dot(val_y, vec3(1./3));
	vec3 y_centered = val_y - y_mean;


	d += (target_sat-length(y_centered))*d_centered;
*/
	float mean = dot(val_new, vec3(1./3));
	vec3 centered = val_new - mean;

	val_new = mix(val_new, 
		centered/(length(centered)+.001)*target_sat + target_mean,
		target_mix);

	val_new += val_agents;//val_new = max(val_new, u2b(val_agents));

	vec3 d = (val_new - val_y);

	//approach the new color
//	vec3 d = (val_new - val_y);// + .2*(sat + norm);// + blur;

	//some kind of color rotation
	d = mix(d, sign(rot)*(d.gbr - d.brg), abs(rot));
	//float _rot = sigmoid(length(val_agents));
	//d = mix(d, sign(_rot)*(d.gbr - d.brg), abs(_rot));

	d *= time_scale;

	//force derivative to zero at bounds
	//d*= exp(-val_y*val_y/bound_gauss);
	d = max(d, -bound_clip-val_y);
	d = min(d, bound_clip-val_y);
	
    outputColor = vec4(d, 1.);
}