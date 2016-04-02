#version 150

uniform sampler2D y;
uniform sampler2D xgrad;
uniform sampler2D ygrad;

//uniform float t;
uniform ivec2 size;

//uniform float disp_exponent;
//uniform float scale;
uniform float warp_grad;
uniform float warp_color;

out vec4 outputColor;

#define PI 3.1415926535897932384626433832795

vec2 invsize = 1./vec2(size);

float sigmoid(in float x){
	return x/(1.+abs(x));
}
vec3 sigmoid(in vec3 x){
	return x/(1.+abs(x));
}

vec3 msigmoid(in vec3 x){
	return x/(1.+length(x));
}
//convert between bipolar [-1, 1] and unipolar [0, 1]
vec3 u2b(in vec3 u){
	return 2.*u-1.;
}
vec3 b2u(in vec3 b){
	return .5*b+.5;
}
vec3 rgb2yuv(in vec3 c){
	vec3 u = b2u(c);
	vec3 yuv;
	yuv[0] = dot(u, vec3(.299, .587, .114));
	yuv[1] = .492*(u.b - yuv[0]);
	yuv[2] = .877*(u.r - yuv[0]);
	return yuv;
}
vec2 color2dir(in vec3 c){
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

vec2 pol2car(in vec2 pol){
	return pol.x*vec2(cos(pol.y), sin(pol.y));
}
vec2 car2pol(in vec2 car){
	return vec2(length(car), atan(car.y, car.x));
}

//use a sample function to handle different texture types+topologies
vec3 sample(in vec2 p, in sampler2D s){
	return texture(s, p*invsize).rgb;
}

//dynamical process in image space, where position influences color
//(by sampling the image) and color influences position
vec2 ascend(in vec2 p, const in vec3 chan){
	vec2 dp_dt;
	mat2x3 grad;
	//maybe something clever to be done with caching these lookups
	//or should GPU already handle that?
	grad[0] = sample(p,xgrad);
	grad[1] = sample(p,ygrad);
	dp_dt = transpose(grad)*chan;
	return dp_dt;///(length(dp_dt)+.001);//unit_square_to_circle(dp_dt);
}

void snake_color(in sampler2D s, inout vec2 p, const in int n, const in float dt){
	for(int i=0; i<n; i++){
		vec3 c = sample(p,s);
		c = sample(p,s); //magically prevents crash
		p+= dt*color2dir(c);
		//p+= dt*rgb2yuv(c).gb;
	}
}

void snake_color_mean(in sampler2D s, inout vec2 p, const in int n, const in float dt){
	vec3 mean = vec3(0.);
	for(int i=1; i<=n; i++){
		mean = mix(mean, sample(p,s), 1./i);
		p+= dt*color2dir(mean);
	}
}
void snake_color_momentum(in sampler2D s, inout vec2 p, const in int n, const in float dt, const in float m){
	vec2 v;
	for(int i=1; i<=n; i++){
		vec2 a = color2dir(sample(p,s));
		if(i==1)
			v = a;
		else v = mix(a, v, m);
		p += dt*v;
	}
}
void snake_color_mm(in sampler2D s, inout vec2 p, const in int n, const in float dt, const in float m){
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
void snake_color_rnn(in sampler2D s, inout vec2 p, const in int n, const in float dt, const in float m){
	vec3 h = vec3(0.);
	for(int i=1; i<=n; i++){
		h = msigmoid(h*m+sample(p,s));
		p+= dt*color2dir(h);
	}
}
void snake_color_rnn_vw(in sampler2D s, inout vec2 p, const in int n, const in float dt, const in vec3 iw, const in vec3 rw){
	vec3 h = vec3(0.);
	for(int i=1; i<=n; i++){
		h = msigmoid(h*rw+sample(p,s)*iw);
		p+= dt*color2dir(h);
	}
}
void snake_color_rnn_mw(in sampler2D s, inout vec2 p, const in int n, const in float dt, const in mat3 w){
	vec3 h = vec3(0.);
	for(int i=1; i<=n; i++){
		h = msigmoid(w*h+sample(p,y));
		p+= dt*color2dir(h);
	}
}

void snake_variance(in sampler2D s, inout vec2 p, const in int n, const in float dt){
	vec3 ref = sample(p, s);
	//ref -= dot(ref, vec3(1./3));
	//ref /= (length(ref)+.001);
	for(int i=1; i<=n; i++){
		p+= dt*ascend(p, -ref);
	}
}
void snake_variance_momentum(in sampler2D s, inout vec2 p, const in int n, const in float dt, const in float m){
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
void snake_variance_mm(in sampler2D s, inout vec2 p, const in int n, const in float dt, const in float m){
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
void snake_variance_mean(in sampler2D s, inout vec2 p, const in int n, const in float dt){
	vec3 mean = vec3(0.);
	for(int i=1; i<=n; i++){
		mean = mix(mean, sample(p,s), 1./i);
		vec3 chan = (mean - dot(mean, vec3(1./3)));
		chan = -chan/(length(chan)+.001);
		p+= dt*ascend(p, chan);
	}
}
void snake_variance_rnn_mw(in sampler2D s, inout vec2 p, const in int n, const in float dt, const in mat3 w){
	vec3 h = vec3(0.);
	for(int i=1; i<=n; i++){
		h = msigmoid(w*h+sample(p,y));
		p+= dt*ascend(p, h);
	}
}

void star_variance(in sampler2D s, inout vec2 p, const in int spokes, const in int steps, const in float dt){
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

	vec2 p = gl_FragCoord.xy;
	vec2 p_old = p;

	float scale_disp = 1.;//pow(scale, disp_exponent);//1./sqrt(scale);

	if(warp_color != 0.){
		snake_color(y,p,1,warp_color);
		//int scsteps = max(1,int(abs(warp_color)));
		//float scdt = scale_disp*warp_color/float(scsteps);
		//snake_color(y, p, scsteps, scdt);
		//snake_color_mm(y, p, scsteps, scdt, .5);
		//snake_color_mean(y, p, scsteps, scdt);
		//snake_color_rnn(y, p, scsteps, scdt, 1.);
		//snake_color_rnn_vw(y, p, scsteps, scdt, _color_proj[0], _color_proj[1]);
		//snake_color_rnn_mw(y, p, scsteps, scdt, _color_proj);
		//p += warp_color*scale_disp*color2dir(sample(p,y));
	}

	if(warp_grad != 0.){
		snake_variance(y,p,1,warp_grad);
		//int svsteps = max(1,int(abs(warp_grad)));
		//float svdt = scale_disp*warp_grad/float(svsteps);
		//snake_variance(y, p, svsteps, svdt);
		//snake_variance_mm(y, p, svsteps, svdt, .2);
		//snake_variance_momentum(y, p, svsteps, svdt, .2);
		//snake_variance_mean(y, p, svsteps, svdt);
		//snake_variance_rnn_mw(y, p, svsteps, svdt, _grad_proj);
		//p += warp_grad*scale_disp*ascend(p, -sample(p,y));
	}

    vec2 dp = (p-p_old);//*scale;

   	outputColor = vec4(dp, 0., 1.);
}