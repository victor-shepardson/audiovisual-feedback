#version 150

uniform sampler2D y;
uniform sampler2D k0;
uniform sampler2D k1;
uniform sampler2D k2;
uniform sampler2D k3;
uniform float dt;
uniform int mode;

out vec4 outputColor;

void main() {
	ivec2 p = ivec2(gl_FragCoord.xy);
	vec3 color_y = texelFetch(y, p, 0).rgb;
	vec3 color_k0 = texelFetch(k0, p, 0).rgb;
	if(mode>0){
		vec3 color_k1 = texelFetch(k1, p, 0).rgb;
		vec3 color_k2 = texelFetch(k2, p, 0).rgb;
		vec3 color_k3 = texelFetch(k3, p, 0).rgb;

		color_y += dt*(1./6.)*(color_k0 + 2.*color_k1 + 2.*color_k2 + color_k3);
	}
	else{
		color_y += dt*color_k0;
	}
    outputColor = vec4(color_y, 1.);
}