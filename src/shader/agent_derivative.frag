#version 150

uniform sampler2D state;
uniform sampler2D agent_input;

uniform float t;
uniform ivec2 size;

uniform float blur_amt;
uniform float fade_time;

out vec4 outputColor;

#define PI 3.1415926535897932384626433832795

vec2 invsize = 1./vec2(size);

//use a sample function to handle different texture types+topologies
vec4 sample(in vec2 p, sampler2D s){
	return texture(s, p*invsize);
}

vec3 bilateral_filter(const in vec2 p, const in float sigma_c, const in float sigma_p){
	vec3 val = sample(p, state).rgb;

	const int kernel_size = 5;
	const int nsamps = kernel_size*kernel_size;
	const float kernel_rsqu = float(nsamps)*.25;
	vec2 coords[nsamps];

	int idx = 0;
	for(int i=0; i<kernel_size; i++){
		for(int j=0; j<kernel_size; j++){
			vec2 coord = vec2(i,j)-vec2(kernel_size/2);
			if(dot(coord,coord)>kernel_rsqu) continue;
			coords[idx] = coord;
			idx++;
		}
	}

	float s_c = .5/(sigma_c*sigma_c);
	float s_p = .5/(sigma_p*sigma_p);

	float z = 0.;
	vec3 color = vec3(0.);

	for(int i=0; i<nsamps; i++){
		if(i>=idx) break;
		vec2 coord = coords[i];
		vec3 c = sample(p+coord, state).rgb;
		vec3 d_c = c-val;
		vec2 d_p = coords[i];
		float w = exp(-s_c*dot(d_c, d_c) - s_p*dot(d_p, d_p));
		color += c*w;
		z += w;
	}

	if(z>0.)
		color/=z;

	return color;
}

vec3 four_point_filter(const in vec2 p){
	vec4 ret = .25*(
		+ sample(p+vec2(0,1), state)
		+ sample(p+vec2(0,-1), state)
		+ sample(p+vec2(1,0), state)
		+ sample(p+vec2(-1,0), state));
	return ret.rgb;
}

void main() {
	vec2 p = gl_FragCoord.xy;

	vec3 val_state = sample(p,state).rgb;
	vec4 val_input = sample(p,agent_input);

	float fade = 1.-pow(2., -fade_time);

	vec3 blurred = mix(val_state, four_point_filter(p), blur_amt);

	vec3 faded = mix(vec3(.5), blurred, fade);

	vec3 new = mix(faded, val_input.rgb, val_input.a);

	//vec3 damp = -epsilon*val_state;

	vec3 d = new - val_state;

    outputColor = vec4(d, 1.);
}