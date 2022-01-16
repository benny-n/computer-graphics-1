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
varying vec3 woodColor;
out vec2 fTex;

float rand3D(in vec3 co){
    return fract(sin(dot(co.xyz ,vec3(12.9898,78.233,144.7272))) * 43758.5453);
}

float simple_interpolate(in float a, in float b, in float x)
{
   return a + smoothstep(0.0,1.0,x) * (b-a);
}

float interpolatedNoise3D(in float x, in float y, in float z)
{
    float integer_x = x - fract(x);
    float fractional_x = x - integer_x;

    float integer_y = y - fract(y);
    float fractional_y = y - integer_y;

    float integer_z = z - fract(z);
    float fractional_z = z - integer_z;

    float v1 = rand3D(vec3(integer_x, integer_y, integer_z));
    float v2 = rand3D(vec3(integer_x+1.0, integer_y, integer_z));
    float v3 = rand3D(vec3(integer_x, integer_y+1.0, integer_z));
    float v4 = rand3D(vec3(integer_x+1.0, integer_y +1.0, integer_z));

    float v5 = rand3D(vec3(integer_x, integer_y, integer_z+1.0));
    float v6 = rand3D(vec3(integer_x+1.0, integer_y, integer_z+1.0));
    float v7 = rand3D(vec3(integer_x, integer_y+1.0, integer_z+1.0));
    float v8 = rand3D(vec3(integer_x+1.0, integer_y +1.0, integer_z+1.0));

    float i1 = simple_interpolate(v1,v5, fractional_z);
    float i2 = simple_interpolate(v2,v6, fractional_z);
    float i3 = simple_interpolate(v3,v7, fractional_z);
    float i4 = simple_interpolate(v4,v8, fractional_z);

    float ii1 = simple_interpolate(i1,i2,fractional_x);
    float ii2 = simple_interpolate(i3,i4,fractional_x);

    return simple_interpolate(ii1 , ii2 , fractional_y);
}

float Noise3D(in vec3 coord, in float wavelength)
{
   return interpolatedNoise3D(coord.x/wavelength, coord.y/wavelength, coord.z/wavelength);
}

float turbulence() {
	return Noise3D(vPosition.xyz,1) + Noise3D(vPosition.xyz,0.5) + Noise3D(vPosition.xyz,0.25) + Noise3D(vPosition.xyz,0.125);
}

vec3 wood_color(float x) {
	vec3 c1 = vec3(0.26, 0.15, 0.1);
	vec3 c2 = vec3(0.64, 0.45,0.28);
	float t = (sin(x) + 1) / 2;
	return t * c1 + ( 1-t ) * c2;
}

vec3 wood() {
	float noisy = pow(vPosition.x,2) + pow(vPosition.y,2) + turbulence();
	return wood_color(noisy);
}

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
    woodColor = wood();
    fTex = tex;
}
