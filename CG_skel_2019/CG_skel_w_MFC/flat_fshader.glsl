#version 150

uniform sampler2D texSampler;

in vec4 out_color;
in vec2 fTex;
out vec4 fColor;

void main()
{
    fColor = texture(texSampler, fTex);
}
