#version 150

uniform sampler2D y;
uniform sampler2D xgrad;
uniform sampler2D ygrad;
uniform sampler2D modulation;

uniform float t;
uniform ivec2 size;

//uniform	float time_scale;
//uniform	float rot;
//uniform	float bound_gauss;
//uniform	float bound_clip;
uniform float disp_exponent;
uniform float scale;
uniform float warp_grad;
uniform float warp_color;
uniform float zoom;
uniform float swirl;
uniform float suck;
uniform vec2 drift;
uniform	float target_sat;
uniform	float target_mean;
uniform float target_mix;
uniform float mirror_shape;
uniform float mirror_amt;

uniform mat4 color_proj;
uniform mat4 grad_proj;

out vec4 outputColor;

#define PI 3.1415926535897932384626433832795

vec2 invsize = 1./vec2(size);

float sigmoid(float x){
	return x/(1.+abs(x));
}
vec3 sigmoid(vec3 x){
	return x/(1.+abs(x));
}

vec3 msigmoid(vec3 x){
	return x/(1.+length(x));
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
vec2 ascend(in vec2 p, const vec3 chan){
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
		p+= dt*color2dir(sample(p,s));
	}
	return sample(p, s);
}

vec3 snake_color_mean(sampler2D s, inout vec2 p, const int n, const float dt){
	vec3 mean = vec3(0.);
	for(int i=1; i<=n; i++){
		mean = mix(mean, sample(p,s), 1./i);
		p+= dt*color2dir(mean);
	}
	return sample(p, s);
}
vec3 snake_color_rnn(sampler2D s, inout vec2 p, const int n, const float dt, const float m){
	vec3 h = vec3(0.);
	for(int i=1; i<=n; i++){
		h = msigmoid(h*m+sample(p,y));
		p+= dt*color2dir(h);
	}
	return sample(p, s);
}
vec3 snake_color_rnn_vw(sampler2D s, inout vec2 p, const int n, const float dt, const vec3 iw, const vec3 rw){
	vec3 h = vec3(0.);
	for(int i=1; i<=n; i++){
		h = msigmoid(h*rw+sample(p,y)*iw);
		p+= dt*color2dir(h);
	}
	return sample(p, s);
}
vec3 snake_color_rnn_mw(sampler2D s, inout vec2 p, const int n, const float dt, const mat3 w){
	vec3 h = vec3(0.);
	for(int i=1; i<=n; i++){
		h = msigmoid(w*h+sample(p,y));
		p+= dt*color2dir(h);
	}
	return sample(p, s);
}

vec3 snake_variance(sampler2D s, inout vec2 p, const int n, const float dt){
	vec3 ref = sample(p, y);
	for(int i=1; i<=n; i++){
		p+= dt*ascend(p, -ref);
	}
	return sample(p,s);
}
vec3 snake_variance_mean(sampler2D s, inout vec2 p, const int n, const float dt){
	vec3 mean = vec3(0.);
	for(int i=1; i<=n; i++){
		mean = mix(mean, sample(p,s), 1./i);
		p+= dt*ascend(p, -mean);
	}
	return sample(p,s);
}
vec3 snake_variance_rnn_mw(sampler2D s, inout vec2 p, const int n, const float dt, const mat3 w){
	vec3 h = vec3(0.);
	for(int i=1; i<=n; i++){
		h = msigmoid(w*h+sample(p,y));
		p+= dt*ascend(p, h);
	}
	return sample(p, s);
}

vec3 star_variance(sampler2D s, inout vec2 p, const int spokes, const int steps, const float dt){
	vec3 mean = vec3(0.);
	vec2 cur_p = p;
	for(int i=1; i<=spokes; i++){
		vec3 cur_c = sample(cur_p,s);
		mean = mix(mean, cur_c, 1./i);
		cur_p = p;
		for(int j=0; j<steps; j++)
			cur_p+= dt*ascend(cur_p, -mean);
	}
	p = cur_p;
	return sample(p,s);
}


void main() {
	mat3 _color_proj, _grad_proj;
	for(int i=0; i<3; i++){
		_color_proj[i] = color_proj[i].xyz;
		_grad_proj[i] = grad_proj[i].xyz;
	}

	vec2 p = gl_FragCoord.xy;
	vec3 val_y = sample(p,y);
	vec3 val_m = val_y;//sample(p, modulation);
	//vec3 val_agents = sigmoid(sample(p, agents));
	
	vec3 val_new;

	//float ss = scale;//sqrt(scale);

	float scale_disp = pow(scale, disp_exponent);//1./sqrt(scale);

	vec2 radial = .5-p*invsize;
	float rdfc = 2.*length(radial);
	radial = normalize(radial);
	vec2 perp_radial = vec2(radial.x, -radial.y);

	float _zoom = pow(2., zoom*val_m.r);
	p = size*(_zoom*(p*invsize-.5)+.5);

	p += suck*scale_disp*radial*val_m.g;
	//p += suck*scale_disp*radial;

	p += swirl*scale_disp*perp_radial*val_m.b;
	//p += swirl*scale_disp*perp_radial;

	//p += drift*scale_disp*sample(p, modulation).rg;
	p += drift*scale_disp;

	int svsteps = int(abs(warp_grad));
	float svdt = scale_disp*warp_grad/float(svsteps);
	//snake_variance_mean(y, p, svsteps, svdt);
	snake_color_rnn_mw(modulation, p, svsteps, svdt, _grad_proj);
	//p += warp_grad*scale_disp*ascend(p, -sample(p,y));

	int scsteps = int(abs(warp_color));
	float scdt = scale_disp*warp_color/float(scsteps);
	//snake_color_mean(y, p, scsteps, scdt);
	//snake_color_rnn(y, p, scsteps, scdt, 1.);
	//snake_color_rnn_vw(y, p, scsteps, scdt, _color_proj[0], _color_proj[1]);
	snake_color_rnn_mw(modulation, p, scsteps, scdt, _color_proj);
	//p += warp_color*scale_disp*color2dir(sample(p,y));

	val_new = sample(p,y);

	//mirror
	vec2 mirror_coord = vec2(size-p);//vec2(size.x-p.x, p.y);
	mirror_coord.y = mix(mirror_coord.y, p.y, mirror_shape);
	vec3 val_mirror = sample(mirror_coord, y);
	float mirror_amt = .5*pow(max(0.,1.-rdfc), 2);
	val_new = mix(val_new, val_mirror, .5*mirror_amt);

	/*vec3 val_knead;
	if(p.x/float(size.x)<.5)
		val_knead = sample(vec2(2.*p.x,p.y),y);
	else
		val_knead = sample(vec2(2.*(float(size.x)-p.x), p.y), y);
	val_new = mix(val_new, val_knead, .5);
	*/

	//knead
	//vec2 p1 = vec2(.667,1.)*p;
	//vec2 p2 = vec2(.667,1.)*vec2(float(size.x)-p.x, p.y);
	//val_new = mix(sample(p1,y), sample(p2,y),.5);


	//int n = 8;//int(sqrt(scale) + 7.);
	//float dt = 1;

	//vec2 pp = p;
	//vec3 sv = snake_variance(y, pp, n, dt);
	//vec3 sv = snake_variance(y,p,n,dt);
	//vec3 sc = snake_color(y, p, n, warp*scale_disp);
	//p += warp*scale_disp*color2dir(sv);
	//vec3 sc = sample(p,y);

	//val_new = sv;//normalize(sc-.5*sv);

	//val_new *= 2.;//log(1.+scale);

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