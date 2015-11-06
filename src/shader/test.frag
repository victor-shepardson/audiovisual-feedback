#version 150

out vec4 outputColor;

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

void main() {
	vec2 p = gl_FragCoord.xy;
	vec3 c = vec3(2.*sin(p.x*.05));

    outputColor = vec4(c, 1.);
}