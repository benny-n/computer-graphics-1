#version 150

uniform sampler2D texSampler;
uniform bool useTex;

in vec4 out_color;
in vec2 fTex;
out vec4 fColor;

void main()
{
    if (useTex) fColor = texture(texSampler, fTex);
    else fColor = out_color;
}
