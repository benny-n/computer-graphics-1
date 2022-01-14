#version 150

uniform mat4 modelview;
uniform mat4 normalview;

in vec3 vPosition;
in vec3 tangent;
in vec3 bitangent;  
in float alpha;
in vec2 tex;

varying vec3 fPosition;
out vec2 fTex;
out mat3 TBN;
out float fAlpha;


void main()
{
    vec3 modifiedPosition = (modelview * vec4(vPosition,1)).xyz;
    gl_Position = vec4(modifiedPosition,1);
    fPosition = modifiedPosition;
    fTex = tex;

    vec3 T = normalize(vec3(normalview * vec4(tangent, 0.0)));
    vec3 B = normalize(vec3(normalview * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(normalview * vec4(0, 0, 1, 0.0)));
    TBN = mat3(T, B, N);
}
