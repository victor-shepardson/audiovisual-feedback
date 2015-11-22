#version 150

uniform sampler2D y;
uniform sampler2D xgrad;
uniform sampler2D ygrad;

uniform float t;
uniform ivec2 size;

//uniform	float time_scale;
//uniform	float rot;
//uniform	float bound_gauss;
//uniform	float bound_clip;
uniform float scale;
uniform float warp;
uniform float zoom;
uniform	float target_sat;
uniform	float target_mean;
uniform float target_mix;

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

/*vec2 color2dir(vec3 c){
	mat2x3 m;
	m[0] = normalize(vec3(1.,-.5,-.5));
	m[1] = normalize(vec3(0.,1.,-1.));
	return c*m;
}*/
vec2 color2dir(vec3 c){
	vec3 px = normalize(vec3(1.,-.5,-.5));
	vec3 py = normalize(vec3(0.,1.,-1.));
	return vec2(dot(c,px),dot(c,py));
}


vec2 pol2car(vec2 pol){
	return pol.x*vec2(cos(pol.y), sin(pol.y));
}
vec2 car2pol(vec2 car){
	return vec2(length(car), atan(car.y, car.x));
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


//dynamical process in image space, where position influences color
//(by sampling the image) and color influences position
vec2 snake_derivative(in vec2 p, const vec3 chan){
	vec2 dp_dt;
	mat2x3 grad;
	//maybe something clever to be done with caching these lookups
	//or should GPU already handle that?
	grad[0] = sample(p,xgrad);
	grad[1] = sample(p,ygrad);
	dp_dt = transpose(grad)*chan;
	return dp_dt;//unit_square_to_circle(dp_dt);
}
/*
void snake_rk_step(inout vec2 p, const float dt, const vec3 chan){
	vec2 k0 = snake_derivative(p, chan);
	vec2 k1 = snake_derivative(p+k0*.5*dt, chan);
	vec2 k2 = snake_derivative(p+k1*.5*dt, chan);
	vec2 k3 = snake_derivative(p+k2*dt, chan);

	p += dt*(1./6)*(k0+2.*k1+2.*k2+k3);
	return;
}
vec3 snake_rk(in vec2 p, const int n, const float dt, const vec3 chan){
	//vec3 acc = sample(p,y);//vec3(0);//
	for(int i=0; i<n; i++){
		snake_rk_step(p,dt,chan);
		//acc = mix(acc, sample(p,y), 1.);
	}
	return sample(p, y);//here returning only color at the end point
}
vec3 snake_dumb(sampler2D s, inout vec2 p, const int n, const float dt, const vec3 chan){
	for(int i=0; i<n; i++){
		p+= dt*snake_derivative(p, chan);
	}
	return sample(p, s);//here returning only color at the end point
}
*/
vec3 snake_color(sampler2D s, inout vec2 p, const int n, const float dt){
	for(int i=0; i<n; i++){
		p+= dt*color2dir(sample(p, s));
	}
	return sample(p, s);
}

vec3 snake_variance(sampler2D s, inout vec2 p, const int n, const float dt){
	vec3 mean = vec3(0.);
	for(int i=1; i<=n; i++){
		mean = mix(mean, sample(p,s), 1./n);
		p+= dt*snake_derivative(p, -mean);
	}
	return sample(p,s);
}

void main() {
/*	mat3 _color_proj, _grad_proj;
	for(int i=0; i<3; i++){
		_color_proj[i] = color_proj[i].xyz;
		_grad_proj[i] = grad_proj[i].xyz;
	}*/

	vec2 p = gl_FragCoord.xy;
	vec3 val_y = sample(p,y);
	//vec3 val_agents = sigmoid(sample(p, agents));
	
	vec3 val_new;

	//float ss = scale;//sqrt(scale);

	float scale_disp = 1./(scale);

	p += zoom*scale_disp*normalize(.5*size-p);

	//p += warp*scale_disp*color2dir(sample(p,y));



	int n = int(sqrt(scale) + 4.);
	float dt = 1;

	vec3 sc = snake_color(y, p, n, warp*scale_disp);
	vec3 sv = snake_variance(y, p, n, dt);

	val_new = sv;

	val_new *= 2.;//log(1.+scale);

//	val_new /= sqrt(scale);
/*
	float mean = dot(val_new, vec3(1./3));
	vec3 centered = val_new - mean;

	val_new = mix(val_new, 
		centered/(length(centered)+.001)*target_sat + target_mean,
		target_mix);
*/
	//val_new = max(val_new, u2b(val_agents));

	//approach the new color
	//vec3 d = (val_new - val_y);// + .2*(sat + norm);// + blur;

    outputColor = vec4(val_new, 1.);
}