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

vec3 bilateral_filter(const in float sigma_c, const in float sigma_p){

	vec2 p = gl_FragCoord.xy;

	vec3 val = sample(p);

	const int kernel_size = 7;
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
		vec3 c = sample(p+coord);
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

void main() {

	const float sigma_c = .15;
	const float sigma_p = 1.;

	vec3 color = bilateral_filter(sigma_c, sigma_p);

	outputColor = vec4(color, 1.);
}
