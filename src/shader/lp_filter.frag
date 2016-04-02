#version 150

uniform sampler2D x;
uniform sampler2D y;

uniform float t;
uniform ivec2 size;

uniform float alpha;
uniform float beta;
uniform float epsilon;
uniform float time_scale;

out vec4 outputColor;

#define PI 3.1415926535897932384626433832795

vec2 invsize = 1./vec2(size);

//use a sample function to handle different texture types+topologies
vec3 sample(in vec2 p, sampler2D s){
	return texture(s, p*invsize).rgb;
}

vec3 bilateral_filter(const in vec2 p, const in float sigma_c, const in float sigma_p){
	vec3 val = sample(p, y);

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
		vec3 c = sample(p+coord, y);
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
	vec3 ret = .25*(
		+ sample(p+vec2(0,1), y)
		+ sample(p+vec2(0,-1), y)
		+ sample(p+vec2(1,0), y)
		+ sample(p+vec2(-1,0), y));
	return ret;
}

void main() {
	vec2 p = gl_FragCoord.xy;

	vec3 val_y = sample(p,y);
	vec3 val_x = sample(p,x);

	vec3 time_filter = alpha*.5*(val_x-val_y);

	vec3 diffusion = .5*beta* //(bilateral_filter(p, .1, .5)-val_y);
		(four_point_filter(p)-val_y);

	vec3 damp = -epsilon*val_y;

	vec3 d = time_scale * (time_filter + diffusion + damp);

    outputColor = vec4(d, 1.);
}