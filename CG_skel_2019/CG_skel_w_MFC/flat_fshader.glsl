#version 150

uniform sampler2D texSampler;
uniform bool useTex;

in vec4 out_color;
in vec2 fTex;
out vec4 fColor;

void main()
{
    fColor = useTex? out_color * texture(texSampler, fTex) : out_color;
}
