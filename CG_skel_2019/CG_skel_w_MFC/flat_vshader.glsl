#version 150


uniform mat4 modelview;
in vec4 vPosition;
//in vec3 in_color;
out vec4 out_color;

void main()
{
    gl_Position = modelview * vPosition;
    out_color = vec4(1,0,0,1);
}
