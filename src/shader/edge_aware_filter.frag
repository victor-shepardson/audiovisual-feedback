#version 150

uniform sampler2D state;
uniform float dir;
uniform ivec2 size;

out vec4 outputColor;

#define PI 3.1415926535897932384626433832795

vec2 invsize = 1./vec2(size);

//use a sample function to handle different texture types+topologies
vec3 sample(in vec2 p){
	return texture(state, p*invsize).rgb;
}

void main() {

	vec2 p = gl_FragCoord.xy;

	vec3 val = sample(p);

	vec3 samps[4];
	samps[0] = sample(p+vec2(0.,1.));
	samps[1] = sample(p+vec2(0.,-1.));
	samps[2] = sample(p+vec2(-1.,0.));
	samps[3] = sample(p+vec2(1.,0.));

	float epsilon = .001;
	float w = 0.;
	float w_floor;
	vec3 c = vec3(0.);
	for(int i=0; i<4; i++){
		vec3 d = samps[i]-val;
		float f = dot(d,d);
		if(i==0 || f<w_floor)
			w_floor = f;
	}
	for(int i=0; i<4; i++){
		vec3 d = samps[i]-val;
		float tw = 1./(dot(d,d)-w_floor+epsilon);
		w+=tw;
		c+=tw*samps[i];
	}

	c = .5*(c/w+val);

	/*vec3 gradx = sample(p+vec2(1.,0.)) - sample(p+vec2(-1.,0.));
	vec3 grady = sample(p+vec2(0.,1.)) - sample(p+vec2(0.,-1.));

	vec2 g;
	g.x = dot(gradx,vec3(1.));
	g.y = dot(grady,vec3(1.));
	g /= (length(g)+.001);
	vec2 perp = vec2(g.y, -g.x);

	vec3 c = .5*mix(sample(p+g)+sample(p-g), sample(p+perp)+sample(p-perp), dir);
*/
    outputColor = vec4(c, 1.);
}
