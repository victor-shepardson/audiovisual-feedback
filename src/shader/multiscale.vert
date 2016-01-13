//dummy vertex shader; we will use gl_FragCoord in the fragment shader

/*void main() {
    gl_Position = ftransform();
}*/

#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
 
void main(){
    gl_Position = modelViewProjectionMatrix * position;
}