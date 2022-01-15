#version 150

uniform mat4 transform;

in vec3 vPosition;

varying vec3 tex;

void main() {
    tex = vPosition;
    gl_Position = transform * vec4(vPosition, 1.0);
}
