#version 150

uniform mat4 modelview;

in vec3 vPosition;
in vec3 vNormal;

void main() {
	vec3 offset_position = vPosition + 0.15 * vNormal;
	gl_Position = modelview * vec4(offset_position,1);
}