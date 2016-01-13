#version 150

uniform sampler2D x;
uniform sampler2D y;
uniform float a;
uniform float b;
uniform ivec2 size;

out vec4 outputColor;

vec2 invsize = 1./vec2(size);

// ax+by

vec3 sample(in vec2 p, sampler2D s){
	return texture(s, p*invsize).rgb;
}

void main() {
	vec2 p = gl_FragCoord.xy;

    outputColor = vec4(a*sample(p,x)+b*sample(p,y), 1.);
}
