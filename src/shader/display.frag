#version 150

uniform sampler2D state;
uniform ivec2 size;

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
	return texture(s, p*invsize).rgb;
}

void main() {
	vec3 color = sample(gl_FragCoord.xy, state);

	//color = sigmoid(3.*(2.*color-1.))*.5+.5;

	//color = mod(p,2.).xyx;

	/*vec3 c0 = vec3(.0,.0,.0);
	vec3 c1 = vec3(.66,.66,.66);
	vec3 c2 = vec3(.33,.33,.33);
	vec3 c3 = vec3(1.,1.,1.);

	vec3 cr = mix(c0,c1,color.r);
	vec3 cg = mix(c2,c3,color.g);
	vec3 cb = mix(cr,cg,color.b);
*/
	//color = sin(p.xxx*.05);

	//color = clamp(color, -PI*.5, PI*.5);
	//color = sin(color);
	color = clamp(color, -1., 1.);
	color = b2u(color);

    outputColor = vec4(color, 1.);
}