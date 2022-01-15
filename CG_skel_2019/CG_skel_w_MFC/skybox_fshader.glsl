#version 150

uniform samplerCube skybox;

varying vec3 tex;

out vec4 fColor;

void main() {    
    fColor = texture(skybox, tex);
}