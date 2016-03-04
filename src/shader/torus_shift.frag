#version 150

uniform sampler2D state;
uniform ivec2 size;
uniform vec2 shift;

#define PI 3.1415926535897932384626433832795

out vec4 outputColor;

vec2 invsize = 1./vec2(size);

vec3 sigmoid(vec3 x){
	return x/(1.+abs(x));
}
//convert between bipolar [-1, 1] and unipolar [0, 1]
vec3 u2b(vec3 u){
	return 2.*u-1.;
}
vec3 b2u(vec3 b){
	return .5*b+.5;
}

vec3 sample(in vec2 p, sampler2D s){
	return texture(s, p*invsize+shift).rgb;
}

void main() {
	vec3 color = sample(gl_FragCoord.xy, state);

    outputColor = vec4(color, 1.);
}