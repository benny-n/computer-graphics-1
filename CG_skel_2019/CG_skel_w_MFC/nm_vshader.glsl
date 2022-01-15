#version 150

uniform mat4 modelview;
uniform mat4 normalview;

in vec3 vPosition;
in vec3 tangent;
in vec3 bitangent;  
in vec3 ka;
in vec3 kd;
in vec3 ks;
in vec3 emission;
in float alpha;
in vec2 tex;

varying vec3 fPosition;
varying vec3 fKa;
varying vec3 fKd;
varying vec3 fKs;
varying vec3 fEmission;
varying float fAlpha;
out vec2 fTex;
out mat3 TBN;


void main()
{
    vec3 modifiedPosition = (modelview * vec4(vPosition,1)).xyz;
    gl_Position = vec4(modifiedPosition,1);
    fPosition = modifiedPosition;
    fKa = ka;
    fKd = kd;
    fKs = ks;
    fEmission = emission;
    fAlpha = alpha;
    fTex = tex;

    vec3 T = normalize(vec3(normalview * vec4(tangent, 0.0)));
    vec3 B = normalize(vec3(normalview * vec4(bitangent, 0.0)));
    vec3 N = normalize(cross(T,B));
    TBN = mat3(T, B, N);
}
