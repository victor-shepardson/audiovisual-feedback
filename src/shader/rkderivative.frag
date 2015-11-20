#version 150

uniform sampler2D y;
uniform sampler2D xgrad;
uniform sampler2D ygrad;
uniform sampler2D agents;
uniform sampler2D blur0;
uniform sampler2D blur1;
uniform sampler2D blur2;
uniform sampler2D blur3;

uniform float t;
uniform ivec2 size;

uniform	float target_sat;
uniform	float target_mean;
uniform float target_mix;
uniform	float time_scale;
uniform	float rot;
uniform	float bound_gauss;
uniform	float bound_clip;
uniform float warp;
uniform float zoom;

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

vec2 color2dir(vec3 c){
	mat2x3 m;
	m[0] = normalize(vec3(1.,-.5,-.5));
	m[1] = normalize(vec3(0.,1.,-1.));
	return c*m;
}
/*
//hsv conversion from lolengine.net: http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}
*/
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
vec3 snake_color(in vec2 p, const int n, const float dt){
	for(int i=0; i<n; i++){
		p+= dt*color2dir(sample(p, y));
	}
	return sample(p, y);
}
vec3 snake_variance(sampler2D s, inout vec2 p, const int n, const float dt){
	vec3 mean = vec3(0.);
	for(int i=1; i<=n; i++){
		mean = mix(mean, sample(p,s), 1./n);
		p+= dt*snake_derivative(p, -mean);
	}
	return sample(p,s);
}

/*vec2 snake_derivative(in vec2 p, in vec3 c, out vec3 cprime, const vec3 chan){
	vec2 dp_dt;
	vec3 color = sample(p,y);
	cprime = 1.*(color-c);////sin(2.*(color-c));//sigmoid(4.*(color - c));//
	mat2x3 grad;
	grad[0] = sample(p,xgrad);
	grad[1] = sample(p,ygrad);
	dp_dt = transpose(grad)*chan;
	return dp_dt;//unit_square_to_circle(dp_dt);
}
void snake_rk_step(inout vec2 p, inout vec3 c, const float dt, const vec3 chan){
	vec3 c0,c1,c2,c3;
	//ended up just hacking around proper integration for color; not worth it?
	vec2 k0 = snake_derivative(p, c, c0, chan);
	vec2 k1 = snake_derivative(p+k0*.5*dt, c+c0*.5, c1, chan);
	vec2 k2 = snake_derivative(p+k1*.5*dt, c+c1*.5, c2, chan);
	vec2 k3 = snake_derivative(p+k2*dt, c+c2, c3, chan);

	c += dt*(1./6)*(c0+2.*c1+2.*c2+c3);
	p += dt*(1./6)*(k0+2.*k1+2.*k2+k3);
	return;
}
vec3 snake_rk(in vec2 p, const int n, const float dt, const vec3 chan){
	vec3 c = sample(p,y);//vec3(0);//
	for(int i=0; i<n; i++){
		snake_rk_step(p,c,dt,chan);
	}
	return c;
}
*/

void main() {
	mat3 _color_proj, _grad_proj;
	for(int i=0; i<3; i++){
		_color_proj[i] = color_proj[i].xyz;
		_grad_proj[i] = grad_proj[i].xyz;
	}

	vec2 p = gl_FragCoord.xy;
	vec3 val_y = texelFetch(y, ivec2(p), 0).rgb;
	vec3 val_agents = sigmoid(sample(p, agents));

	
	vec3 val_new;

	//domain distortion stage
	/*mat2x3 grad;
	grad[0] = texelFetch(xgrad,ivec2(p),0).rgb;
	grad[1] = texelFetch(ygrad,ivec2(p),0).rgb;
	vec2 ofs;// = vec2(8);
	for(int i=0; i<2; i++)
		ofs[i] = warp*(1./3)*(grad[i].r+grad[i].g+grad[i].b);
	//ofs.y*=-1.;
	p += ofs;
	*/
	//vec3 hsv = rgb2hsv(sample(p,y));
	//vec2 ofs = warp*pol2car(vec2(hsv.y*hsv.z, 2.*PI*hsv.x));
	//vec3 color_blur = b2u(val_blur);
	//vec2 ofs = warp*color2dir(val_blur);
//	p+=ofs;

//	val_new = sample(p, blur);
	//val_new = snake_color(p, 8, warp);

	p += zoom*normalize(.5*size-p);

	int steps = 24;
	float dt = warp;
	vec3 val_var = snake_variance(y,p,steps,dt);
	
/*
	vec2 ofs0 = warp*color2dir(sample(p,blur0));
	vec2 ofs1 = 3.*warp*color2dir(sample(p,blur1));
	vec2 ofs2 = 9.*warp*color2dir(sample(p,blur2));
	vec2 ofs3 = 27.*warp*color2dir(sample(p,blur3));
*/

	/*p += 27.*warp*color2dir(sample(p,blur3));
	p += 9.*warp*color2dir(sample(p,blur2));
	p += 3.*warp*color2dir(sample(p,blur1));
	p += warp*color2dir(sample(p,blur0));
*/
/*
	p += warp*color2dir(sample(p,blur0));
	p += 3.*warp*color2dir(sample(p,blur1));
	p += 9.*warp*color2dir(sample(p,blur2));
	p += 27.*warp*color2dir(sample(p,blur3));
*/
	vec3 c;
	vec2 ofs;

	val_new = vec3(0.);//sample(p,blur0)+sample(p,blur1)+sample(p,blur2)+sample(p,blur3);//vec3(0);//sample(p,blur3);//

	c = sample(p,blur0);
	ofs = warp*color2dir(c);
	val_new += sample(p+ofs,y);//-sample(p+ofs,blur1);
/*
	c = sample(p,blur1);
	ofs = 3.*warp*color2dir(c);
	val_new += sample(p+ofs,y);//-sample(p+ofs,blur2);

	c = sample(p,blur2);
	ofs = 9.*warp*color2dir(c);
	val_new += sample(p+ofs,y);//-sample(p+ofs,blur3);

	c = sample(p,blur3);
	ofs = 27.*warp*color2dir(c);
	val_new += sample(p+ofs,y);

	val_new *= .25;
*/

/*	val_new = sample(p+ofs0, blur0)
			+ sample(p+ofs1, blur1)
			+ sample(p+ofs2, blur2);
			+ sample(p+ofs3, blur3);

	val_new *=.25;
	*/
	//val_new = sample(p,y);

	p += ofs;

	//dynamic snakes
	//dt of about 2 actually seems best, and small number of steps works fine
	//int steps = 8;
	//float dt = 1;
	/*mat3 snake_colors; //grad_proj,color_proj,
	for(int i=0; i<3; i++)
		snake_colors[i] = snake_dumb(p,steps,dt, _grad_proj[i]);
	snake_colors *= _color_proj;
	for(int i=0; i<3; i++)
		val_new[i] = snake_colors[i][i];
	*/
	
	/*vec3 peak = climb(p,steps,dt);
	vec3 trough = climb(p,steps,-dt);

	float local_contrast = length(peak-trough)/sqrt(3.);
	val_new = mix(val_y, val_blur, local_contrast);
*/
/*
	int steps = 8;
	float dt = 1;
	vec3 local_mean = val_y;
	int num = 1;
	//vec3 peak0 = val_y;
	vec3 peak1 = snake_dumb(y,p,steps,dt,-local_mean);
	local_mean = mix(local_mean, peak1, 1./(++num));
	vec3 peak2 = snake_dumb(y,p,steps,dt,-local_mean);
	local_mean = mix(local_mean, peak2, 1./(++num));
	vec3 peak3 = snake_dumb(y,p,steps,dt,-local_mean);
	local_mean = mix(local_mean, peak3, 1./(++num));

	/*vec3 peaks;
	peaks.r = length(peak1-local_mean);
	peaks.g = length(peak2-local_mean);
	peaks.b = length(peak3-local_mean);
	peaks = sigmoid(_color_proj*peaks);
*/
//	val_new = .5*(val_new + peak3) + val_agents;
/*
	int steps = 24;
	float dt = warp;
	val_new = snake_variance(y,p,steps,dt);
*/	
	//val_new += val_agents;

	float mean = dot(val_new, vec3(1./3));
	vec3 centered = val_new - mean;

	val_new = mix(val_new, 
		centered/(length(centered)+.001)*target_sat + target_mean,
		target_mix);

	val_new = max(val_new, u2b(val_agents));

	//approach the new color
	vec3 d = (val_new - val_y);// + .2*(sat + norm);// + blur;

	//some kind of color rotation
	d = mix(d,sign(rot)*(d.gbr - d.brg),abs(rot));
	//float _rot = sigmoid(length(val_agents));
	//d = mix(d, sign(_rot)*(d.gbr - d.brg), abs(_rot));

	d*= time_scale;

	//force derivative to zero at bounds
	//d*= exp(-val_y*val_y/bound_gauss);
	d = max(d, -bound_clip-val_y);
	d = min(d, bound_clip-val_y);
	
    outputColor = vec4(d, 1.);
}