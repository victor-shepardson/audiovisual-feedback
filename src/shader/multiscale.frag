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
//uniform	float target_sat;
//uniform	float target_mean;
//uniform float target_mix;
//uniform float mirror_shape;
//uniform float mirror_amt;

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
	vec3 u = b2u(c);
	vec3 ycm = min(u.rgr, u.gbb);
	vec3 rgb = u - max(ycm.rrg, ycm.bgb);
	vec2 h;
	h.x = dot(rgb,vec3(1.,-.5,-.5)) + dot(ycm,vec3(.5,-1.,.5));
	h.y = sqrt(3.)*.5*(dot(rgb,vec3(0.,1.,-1.)) + dot(ycm,vec3(1.,0.,-1.)));
	float sat = max(u.r,max(u.b,u.g)) - min(u.r,min(u.b,u.g));
	return h/(length(h)+.001) * sat;
	//vec3 px = normalize(vec3(1.,-.5,-.5));
	//vec3 py = normalize(vec3(0.,1.,-1.));
	//return vec2(dot(c,px),dot(c,py));
}

vec2 pol2car(vec2 pol){
	return pol.x*vec2(cos(pol.y), sin(pol.y));
}
vec2 car2pol(vec2 car){
	return vec2(length(car), atan(car.y, car.x));
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
	return dp_dt;///(length(dp_dt)+.001);//unit_square_to_circle(dp_dt);
}

void snake_color(sampler2D s, inout vec2 p, const int n, const float dt){
	for(int i=0; i<n; i++){
		p+= dt*color2dir(sample(p,s));
	}
}

void snake_color_mean(sampler2D s, inout vec2 p, const int n, const float dt){
	vec3 mean = vec3(0.);
	for(int i=1; i<=n; i++){
		mean = mix(mean, sample(p,s), 1./i);
		p+= dt*color2dir(mean);
	}
}
void snake_color_momentum(sampler2D s, inout vec2 p, const int n, const float dt, const float m){
	vec2 v;
	for(int i=1; i<=n; i++){
		vec2 a = color2dir(sample(p,s));
		if(i==1)
			v = a;
		else v = mix(a, v, m);
		p += dt*v;
	}
}
void snake_color_mm(sampler2D s, inout vec2 p, const int n, const float dt, const float m){
	vec2 v;
	vec3 mean = vec3(0.);
	for(int i=1; i<=n; i++){
		mean = mix(mean, sample(p,s), 1./i);
		vec2 a = color2dir(mean);
		if(i==1)
			v = a;
		else v = mix(a, v, m);
		p += dt*v;
	}
}
void snake_color_rnn(sampler2D s, inout vec2 p, const int n, const float dt, const float m){
	vec3 h = vec3(0.);
	for(int i=1; i<=n; i++){
		h = msigmoid(h*m+sample(p,s));
		p+= dt*color2dir(h);
	}
}
void snake_color_rnn_vw(sampler2D s, inout vec2 p, const int n, const float dt, const vec3 iw, const vec3 rw){
	vec3 h = vec3(0.);
	for(int i=1; i<=n; i++){
		h = msigmoid(h*rw+sample(p,s)*iw);
		p+= dt*color2dir(h);
	}
}
void snake_color_rnn_mw(sampler2D s, inout vec2 p, const int n, const float dt, const mat3 w){
	vec3 h = vec3(0.);
	for(int i=1; i<=n; i++){
		h = msigmoid(w*h+sample(p,y));
		p+= dt*color2dir(h);
	}
}

void snake_variance(sampler2D s, inout vec2 p, const int n, const float dt){
	vec3 ref = sample(p, s);
	//ref -= dot(ref, vec3(1./3));
	//ref /= (length(ref)+.001);
	for(int i=1; i<=n; i++){
		p+= dt*ascend(p, -ref);
	}
}
void snake_variance_momentum(sampler2D s, inout vec2 p, const int n, const float dt, const float m){
	vec3 ref = sample(p, s);
	//ref = (ref-dot(ref, vec3(1./3)))/(length(ref)+.001);
	vec2 v;
	for(int i=1; i<=n; i++){
		vec2 a = ascend(p, -ref);
		if(i==1)
			v = a;
		else v = mix(a, v, m);
		p+= dt*v;
	}
}
void snake_variance_mm(sampler2D s, inout vec2 p, const int n, const float dt, const float m){
	vec2 v ;
	vec3 mean = vec3(0.);
	for(int i=1; i<=n; i++){
		mean = mix(mean, sample(p,s), 1./i);
		vec3 chan = (mean - dot(mean, vec3(1./3)));
		chan = -chan/(length(chan)+.001);
		vec2 a = ascend(p, chan);
		if(i==1)
			v = a;
		else v = mix(a, v, m);
		p+= dt*v;
	}
}
void snake_variance_mean(sampler2D s, inout vec2 p, const int n, const float dt){
	vec3 mean = vec3(0.);
	for(int i=1; i<=n; i++){
		mean = mix(mean, sample(p,s), 1./i);
		vec3 chan = (mean - dot(mean, vec3(1./3)));
		chan = -chan/(length(chan)+.001);
		p+= dt*ascend(p, chan);
	}
}
void snake_variance_rnn_mw(sampler2D s, inout vec2 p, const int n, const float dt, const mat3 w){
	vec3 h = vec3(0.);
	for(int i=1; i<=n; i++){
		h = msigmoid(w*h+sample(p,y));
		p+= dt*ascend(p, h);
	}
}

void star_variance(sampler2D s, inout vec2 p, const int spokes, const int steps, const float dt){
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
}


void main() {
	/*mat3 _color_proj, _grad_proj;
	for(int i=0; i<3; i++){
		_color_proj[i] = color_proj[i].xyz;
		_grad_proj[i] = grad_proj[i].xyz;
	}*/

	vec2 p = gl_FragCoord.xy;
	vec2 p_old = p;
	vec3 val_y = sample(p,y);
	vec3 val_m = sample(p, modulation);
	//vec3 val_agents = sigmoid(sample(p, agents));
	
	vec3 val_new;

	p += val_m.xy/scale;

	//float ss = scale;//sqrt(scale);

	float scale_disp = pow(scale, disp_exponent);//1./sqrt(scale);

	int scsteps = int(abs(warp_color));
	float scdt = scale_disp*warp_color/float(scsteps);
	snake_color(y, p, scsteps, scdt);
	//snake_color_mm(y, p, scsteps, scdt, .5);
	//snake_color_mean(y, p, scsteps, scdt);
	//snake_color_rnn(y, p, scsteps, scdt, 1.);
	//snake_color_rnn_vw(y, p, scsteps, scdt, _color_proj[0], _color_proj[1]);
	//snake_color_rnn_mw(y, p, scsteps, scdt, _color_proj);
	//p += warp_color*scale_disp*color2dir(sample(p,y));

	int svsteps = int(abs(warp_grad));
	float svdt = scale_disp*warp_grad/float(svsteps);
	snake_variance(y, p, svsteps, svdt);
	//snake_variance_mm(y, p, svsteps, svdt, .2);
	//snake_variance_momentum(y, p, svsteps, svdt, .2);
	//snake_variance_mean(y, p, svsteps, svdt);
	//snake_variance_rnn_mw(y, p, svsteps, svdt, _grad_proj);
	//p += warp_grad*scale_disp*ascend(p, -sample(p,y));

	//val_new = sample(p,y);

	//mirror
	//vec2 mirror_coord = vec2(size-p);//vec2(size.x-p.x, p.y);
	//mirror_coord.y = mix(mirror_coord.y, p.y, mirror_shape);
	//vec3 val_mirror = sample(mirror_coord, y);
	//float _mirror_amt = mirror_amt*.5*pow(max(0.,1.-rdfc), 2);
	//val_new = mix(val_new, val_mirror, _mirror_amt);

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

    //outputColor = vec4(val_new, 1.);

    vec2 dp = (p-p_old)*scale;

   	outputColor = vec4(dp, 0., 1.);
}