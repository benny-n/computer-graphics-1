#version 150

uniform samplerCube skybox;
uniform sampler2D texSampler;
uniform bool useTex;
uniform float reflectivity;
uniform vec3 eye;

in vec4 out_color;
in vec3 fPosition;
in vec3 fNormal;
in vec2 fTex;

out vec4 fColor;

void main()
{
    vec4 base_color = useTex? out_color * texture(texSampler, fTex) : out_color;
    vec3 I = normalize(fPosition - eye);
    vec3 R = reflect(I, normalize(fNormal));
    vec4 reflective_color = reflectivity != 0? vec4(texture(skybox, R).rgb, 1.0) : vec4(0,0,0,0);
    fColor = reflectivity * reflective_color + (1 - reflectivity) * base_color;
}
