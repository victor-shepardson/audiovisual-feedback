#version 150

uniform sampler2D y;
uniform sampler2D yprime;
uniform sampler2D agents;
uniform sampler2D agradx;
uniform sampler2D agrady;

uniform float t;
uniform ivec2 size;

uniform float drive;
uniform float agent_drive;
uniform float warp_agent;
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

vec2 color2dir(vec3 c){
	vec3 px = normalize(vec3(1.,-.5,-.5));
	vec3 py = normalize(vec3(0.,1.,-1.));
	return vec2(dot(c,px),dot(c,py));
}

//use a sample function to handle different texture types+topologies
vec3 sample(in vec2 p, sampler2D s){
	return texture(s, p*invsize).rgb;
}

vec2 ascend(in vec2 p, const vec3 chan){
	vec2 dp_dt;
	mat2x3 grad;
	grad[0] = sample(p,agradx);
	grad[1] = sample(p,agrady);

	dp_dt.x = dot(chan, grad[0]);
	dp_dt.y = dot(chan, grad[1]);
	return dp_dt;
}


void main() {
	vec2 p = gl_FragCoord.xy;

	vec3 val_y = sample(p,y);

	vec2 dp = warp_agent*msigmoid(agent_drive*ascend(p, vec3(1.)));//sample(p,agents));
	p += dp;///max(1., length(dp));

	vec3 val_new = msigmoid(drive*sample(p,yprime));
	//vec3 val_agents = sigmoid(sample(p, agents));//sin(2.*PI*sigmoid(sample(p,agents)/(2.*PI)));//;

	float mean = dot(val_new, vec3(1./3));
	vec3 centered = val_new - mean;

	val_new = mix(val_new, 
		centered/(length(centered)+.001)*target_sat + target_mean,
		target_mix);

	//val_new += val_agents;
	//val_new = max(val_new, u2b(val_agents));

	vec3 d = (val_new - val_y);// + .5 * u2b(val_agents);

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