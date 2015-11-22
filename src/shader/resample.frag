#version 150

uniform sampler2D src0;
uniform sampler2D src1;
uniform sampler2D src2;

uniform int num_textures;
uniform ivec2 dest_size;
//uniform float kernel_width;

out vec4 dest0;
out vec4 dest1;
out vec4 dest2;

#define PI 3.1415926535897932384626433832795

float kernel_width = PI;

ivec2 src_size = textureSize(src0, 0);
vec2 inv_src_size = 1./src_size;
vec2 inv_dest_size = 1./dest_size;

vec3 sample_src(in vec2 p, sampler2D src){
	return texture(src, p*inv_src_size).rgb;
}

vec3 resample(sampler2D src){
	const int pad = 1;
	vec2 dest_p = gl_FragCoord.xy;
	vec2 src_p = dest_p*inv_dest_size*src_size;
	ivec2 src_p_lower = ivec2(src_p)-pad;
	ivec2 src_p_upper = ivec2(1+src_p)+pad;

	const float r = pad+1;

	vec3 acc = vec3(0.);
	float m = 0.;
	for(int x = src_p_lower.x; x<=src_p_upper.x; x++){
		for(int y = src_p_lower.y; y<=src_p_upper.y; y++){
			ivec2 cur_p = ivec2(x,y);
			float d = min(1.,length(src_p - cur_p)/r);
			float w = .5+.5*cos(PI*d);
			acc += w*sample_src(cur_p, src);
			m += w;
		}		
	}

	return acc/m;
	
/*	vec2 dest_p = gl_FragCoord.xy;// - .5;
	ivec2 src_p_lower = ivec2(-1.+(dest_p-.5)*inv_dest_size*src_size);
	ivec2 src_p_upper = ivec2(2.+(dest_p+.5)*inv_dest_size*src_size);
	vec2 src_p = dest_p*inv_dest_size*src_size;

	vec2 r = max(vec2(2.),((src_size/dest_size)+2.)/2.);//(src_p_upper-src_p_lower)/2.;

	vec3 acc = vec3(0.);
	float m = 0.;
	for(int x = src_p_lower.x; x<=src_p_upper.x; x++){
		for(int y = src_p_lower.y; y<=src_p_upper.y; y++){
			ivec2 cur_p = ivec2(x,y);
			vec2 d = (src_p - cur_p)/r;
			float w = exp(-kernel_width*dot(d,d));
			acc += w*sample_src(cur_p, src);
			m += w;
		}		
	}

	return acc/m;*/
}

void main() {
	int nt = num_textures;
    dest0 = vec4(resample(src0), 1.);
/*    if(--nt<=0) return;
    dest1 = vec4(resample(src1), 1.);
    if(--nt<=0) return;
    dest2 = vec4(resample(src2), 1.);*/
}