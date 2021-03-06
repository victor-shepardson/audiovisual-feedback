#version 150

uniform sampler2D y;
uniform sampler2D new_y;
uniform sampler2D lp;
uniform sampler2D agents;
uniform sampler2D agradx;
uniform sampler2D agrady;

uniform float t;
uniform ivec2 size;

uniform float compress;
uniform float drive;
uniform float agent_drive;
uniform float warp_agent;
uniform float saturate;
uniform float bias;
uniform float gen;
uniform float time_scale;
uniform float rot;
uniform float mirror_shape;
uniform float mirror_amt;
uniform float zoom;
uniform float swirl;
uniform float suck;
uniform float xdrift;
uniform float ydrift;
uniform float bound_clip;

vec2 drift = vec2(xdrift, ydrift);

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
	// vec3 u = b2u(c);
	// vec3 ycm = min(u.rgr, u.gbb);
	// vec3 rgb = u - max(ycm.rrg, ycm.bgb);
	// vec2 h;
	// h.x = dot(rgb,vec3(1.,-.5,-.5)) + dot(ycm,vec3(.5,-1.,.5));
	// h.y = sqrt(3.)*.5*(dot(rgb,vec3(0.,1.,-1.)) + dot(ycm,vec3(1.,0.,-1.)));
	// float sat = max(u.r,max(u.b,u.g)) - min(u.r,min(u.b,u.g));
	// return h/(length(h)+.001) * sat;
	vec3 u = b2u(c);
	vec2 d;
	d.x = c.r - (c.g+c.b)*.5;
	d.y = sqrt(3.)*(c.g-c.b)*.5;
	float s = max(u.r,max(u.b,u.g)) - min(u.r,min(u.b,u.g));
	return s*d/(length(d)+.0001);
}

vec2 color2dir(vec3 c, float s_exp){
	vec3 u = b2u(c);
	vec2 d;
	d.x = c.r - (c.g+c.b)*.5;
	d.y = sqrt(3.)*(c.g-c.b)*.5;
	float s = clamp(max(u.r,max(u.b,u.g)) - min(u.r,min(u.b,u.g)), 0., 1.);
	return pow(s, s_exp)*d/(length(d)+.0001);
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

vec3 harmonic(const in vec2 p, const in vec3 h, const in vec3 v, const in vec3 phi){
	vec3 theta = p.x*h + p.y*v;
	vec3 x = theta+phi;
	return sin(2.*PI*x);
}

void main() {
	vec2 p = gl_FragCoord.xy;

	vec2 radial = .5-p*invsize;
	float rdfc = 2.*length(radial);
	radial = normalize(radial);
	vec2 perp_radial = vec2(radial.x, -radial.y);

	vec3 val_y = sample(p,y);

	p = size*(pow(2.,zoom)*(p*invsize-.5)+.5);

	p += suck*radial;

	p += swirl*perp_radial;

	p += drift;

	vec3 val_lp = sample(p,lp);
	vec3 val_new;

	vec3 val_agents = sample(p + warp_agent*color2dir(val_lp, 2.), agents);

	//if(warp_agent<=0.){
		val_new = sample(p,new_y);
	/*}
	else{
		vec3 agx = sample(p, agradx);
		vec3 agy = sample(p, agrady);
		vec2 agr = vec2(agx.r, agy.r);
		vec2 agg = vec2(agx.g, agy.g);
		vec2 agb = vec2(agx.b, agy.b);
		val_new = vec3(
			sample(p+warp_agent*agr, new_y).r,
			sample(p+warp_agent*agg, new_y).g,
			sample(p+warp_agent*agb, new_y).b
			);
	}*/

	//mirror
	vec2 mirror_coord = vec2(size-p);//vec2(size.x-p.x, p.y);
	mirror_coord.y = mix(mirror_coord.y, p.y, mirror_shape);
	vec3 val_mirror = sample(mirror_coord, new_y);
	float _mirror_amt = mirror_amt*.5*pow(max(0.,1.-rdfc), 2);
	val_new = mix(val_new, val_mirror, _mirror_amt);

	//vec2 dp = warp_agent*msigmoid(agent_drive*ascend(p, vec3(1.)));//sample(p,agents));
	//p += dp;///max(1., length(dp));

	//center square
	/*vec2 pc = p - .5*vec2(size);
	if(max(abs(pc.x), abs(pc.y))<.33*min(size.x,size.y))
		pc*=1.5;
	p = pc+.5*size;
	*/
/*
	//vec3 val_new = sin(PI*drive*sample(p,yprime));
	vec3 val_new = drive*sample(p,ynew);
	//vec3 val_new = u2b(fract(drive*b2u(sample(p,yprime))));

	vec3 val_agents = msigmoid(sample(p, agents));//sin(2.*PI*sigmoid(sample(p,agents)/(2.*PI)));//;

	float mean = dot(val_new, vec3(1./3));
	vec3 centered = val_new - mean;

	val_new = mix(val_new, 
		centered/(length(centered)+.001)*target_sat + target_mean,
		target_mix);

	//val_new += val_agents;
	val_new = max(val_new, u2b(val_agents));

	vec3 d = (val_new - val_y);// + .5 * u2b(val_agents);
*/
	float mean = dot(val_new, vec3(1./3));
	vec3 centered = val_new - mean;

	vec3 sat = saturate*centered/(length(centered)+.001);
	/*val_new = mix(val_new, 
		centered/(length(centered)+.001)*target_sat + target_mean,
		target_mix);
	*/
	//val_new += target_sat*centered/(length(centered)+.001);
	//val_new -= target_mean;

	vec2 pt = p*invsize;
	/*vec2 val_lp2 = color2dir(val_lp);
	vec3 g = vec3(
		sin(2.*PI*(pt.x+val_lp.g)),
		sin(2.*PI*(pt.y+val_lp.b)),
		cos(2.*PI*(pt.x-val_lp.r))
		);
	vec2 g2 = sin(2.*PI*vec2(
		pt.x+val_lp2.y,
		pt.y+val_lp2.x
		));
	g = sigmoid(vec3(
		g.x+g.y+g2.x,
		g.y+g.z+g2.y,
		g.x+g.z+g2.x
		));*/
	/*vec3 h = sign(val_lp.rrg)*floor(.5+3.5*abs(val_lp.rrg));//vec3(1.,0.,-1.);
	vec3 v = sign(val_lp.gbb)*floor(.5+3.5*abs(val_lp.gbb));
	vec3 phi = .5*val_lp.gbr;
	vec3 g = harmonic(pt, h, v, phi);
	g = sigmoid(g.xyx + g.yzz);
	*/
	vec3 g = vec3(0.);
	/*float gd = 2.;
	vec3 g = vec3(
		sigmoid(gd*(val_lp.g-val_lp.b+val_lp2.y)),
		sigmoid(gd*(val_lp.b-val_lp.r+val_lp2.x)),
		sigmoid(gd*(val_lp.r-val_lp.g+val_lp2.x-val_lp2.y))
		);
*/
	float _drive = drive;//*(.5+.5*sin(4.*PI*(pt.y-val_lp.r)));

	//approach the new color
	vec3 d = gen*g + val_new - val_y - _drive*val_lp + sat + bias;

	d = mix(d, d/(length(d)+.001), compress);

	//d += vec3(0.);
	d += agent_drive*sigmoid(val_agents);
	//d += agent_drive*sin(8./3.*PI*sigmoid(sample(p, agents)));

	//some kind of color rotation
	d = mix(d, .5*sign(rot)*(d.gbr - d.brg), abs(rot));
	//float _rot = sigmoid(length(val_agents));
	//d = mix(d, sign(_rot)*(d.gbr - d.brg), abs(_rot));

	d *= time_scale;

	//force derivative to zero at bounds
	//d*= exp(-val_y*val_y/bound_gauss);
	//d = max(d, -bound_clip-val_y);
	//d = min(d, bound_clip-val_y);
	if(dot(d, val_y) > 0 && length(val_y) > bound_clip)
		d /= 2.*(length(val_y)-bound_clip)+1.;
	
    outputColor = vec4(d, 1.);
}