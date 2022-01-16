#version 150
#define AMBIENT 0
#define POINT 1
#define PARALLEL 2
#define MAX_LIGHTS 128

struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 la;
    vec3 ld;
    vec3 ls;
};

uniform int numLights;
uniform Light lights[MAX_LIGHTS];
uniform mat4 modelview;
uniform vec3 eye;
uniform bool useTex;
uniform bool useWood;
in vec4 vPosition;
in vec3 faceCenter;
in vec3 faceNormal;
in vec3 ka;
in vec3 kd;
in vec3 ks;
in vec3 emission;
in float alpha;
in vec2 tex;

out vec4 out_color;
out vec2 fTex;

float rand3D(in vec3 co) {
    return fract(sin(dot(co.xyz ,vec3(12.9898,78.233,144.7272))) * 43758.5453);
}

float simple_interpolate(in float a, in float b, in float x) {
   return a + smoothstep(0.0,1.0,x) * (b-a);
}

float interpolatedNoise3D(in float x, in float y, in float z) {
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

float Noise3D(in vec3 coord, in float wavelength) {
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

vec3 calcColor() {
	vec3 actualKa = ka;
	vec3 actualKd = kd;
	vec3 actualKs = ks;
	if (useTex || useWood) {
		actualKa = vec3(1,1,1);
		actualKd = vec3(1,1,1);
		actualKs = vec3(1,1,1);
	}
	vec3 color = vec3(0, 0, 0);
	vec3 v = normalize(eye - faceCenter);
	vec3 n = faceNormal - faceCenter;
	vec3 l, r;

	for (int i = 0; i < numLights; i++) {
		switch (lights[i].type) {
			case AMBIENT:
				l = v;
				break;
			case POINT: 
				l = normalize(lights[i].position - faceCenter);
				break;
			case PARALLEL: 
				l = normalize(-lights[i].direction);
				break;
			default:
			return vec3(1,0,0);
		}
		r = normalize(2 * n * (dot(n, l)) - l);
		vec3 Ia = actualKa * lights[i].la;
		vec3 Id = actualKd * max(0, dot(l, n)) * lights[i].ld;
		vec3 Is = dot(n, l) < 0? vec3(0,0,0) : actualKs * pow(max(0, dot(r, v)), alpha) * lights[i].ls;
		color = color + Ia + Id + Is;
	}

	if (useTex) color += emission;
	return color;
}

void main()
{
    gl_Position = modelview * vPosition;
	vec4 base_color = vec4(calcColor(),1);
	out_color = useWood? base_color * vec4(wood(),1) : base_color; 
	fTex = tex;
}
