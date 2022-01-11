#version 150

uniform mat4 modelview;
uniform mat4 normalview;

in vec3 vPosition;
in vec3 vNormal;
in vec3 ka;
in vec3 kd;
in vec3 ks;
in vec3 emission;
in float alpha;
in vec2 tex;

varying vec3 fPosition;
varying vec3 fNormal;
varying vec3 fKa;
varying vec3 fKd;
varying vec3 fKs;
varying vec3 fEmission;
varying float fAlpha;
out vec2 fTex;


void main()
{
    vec3 modifiedPosition = (modelview * vec4(vPosition,1)).xyz;
	vec3 modifiedNormal = (normalview * vec4(vNormal,1)).xyz;
    gl_Position = vec4(modifiedPosition,1);
    fPosition = modifiedPosition;
    fNormal = modifiedNormal;
    fKa = ka;
    fKd = kd;
    fKs = ks;
    fEmission = emission;
    fAlpha = alpha;
    fTex = tex;
}
