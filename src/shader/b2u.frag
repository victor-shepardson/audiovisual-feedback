#version 150

uniform sampler2D x;

uniform float t;
uniform ivec2 size;

out vec4 outputColor;

#define PI 3.1415926535897932384626433832795

vec2 invsize = 1./vec2(size);

void main() {
    outputColor = .5+.5*texelFetch(x, ivec2(gl_FragCoord.xy), 0);
}