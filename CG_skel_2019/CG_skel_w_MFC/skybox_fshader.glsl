#version 150

uniform samplerCube skybox;

in vec3 tex;

out vec4 fColor;

void main() {    
    fColor = texture(skybox, tex);
}