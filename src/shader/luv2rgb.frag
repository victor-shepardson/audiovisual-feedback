#version 150

uniform sampler2D x;

uniform float t;
uniform ivec2 size;

out vec4 outputColor;

#define PI 3.1415926535897932384626433832795

vec2 invsize = 1./vec2(size);

vec4 sample(in vec2 p, sampler2D s){
	return texture(s, p*invsize);
}

float sigmoid(float x){
	return x/(1.+abs(x));
}

void main() {
	vec2 p = gl_FragCoord.xy;
    vec4 luv = sample(p, x);
    float l = sigmoid(2.*luv.x*luv.x);
    float u = .3*luv.y;
    float v = .3*luv.z;

    vec4 c;
    c.b = l + u/3. - v/sqrt(3.);
    c.g = c.b + v*2./sqrt(3.);
    c.r = l*3. - c.g - c.b; 
    c.a = 1.;

    outputColor = c;
}