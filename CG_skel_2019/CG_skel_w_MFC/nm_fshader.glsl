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
uniform vec3 eye;
uniform samplerCube skybox;
uniform sampler2D texSampler;
uniform bool useTex;
uniform bool useWood;
uniform sampler2D normalMap;
uniform float reflectivity;

varying vec3 fPosition;
varying vec3 fKa;
varying vec3 fKd;
varying vec3 fKs;
varying vec3 fEmission;
varying float fAlpha;
varying vec2 fTex;
varying vec3 woodColor;
in mat3 TBN;

out vec4 fColor;

vec3 calcColor(vec3 normal) {
vec3 actualKa = fKa;
	vec3 actualKd = fKd;
	vec3 actualKs = fKs;
	if (useTex || useWood) {
		actualKa = vec3(1,1,1);
		actualKd = vec3(1,1,1);
		actualKs = vec3(1,1,1);
	}
	vec3 color = vec3(0, 0, 0);
	vec3 v = normalize(eye - fPosition);
	vec3 n = normal;
	vec3 l, r;

	for (int i = 0; i < numLights; i++) {
		switch (lights[i].type) {
			case AMBIENT:
				l = v;
				break;
			case POINT: 
				l = normalize(lights[i].position - fPosition);
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
		vec3 Is = dot(n, l) < 0? vec3(0,0,0) : actualKs * pow(max(0, dot(r, v)), fAlpha) * lights[i].ls;
		color = color + Ia + Id + Is;
	}

	return color;
}

void main() {
	vec3 normal = normalize(TBN * (texture(normalMap, fTex).rgb * 2.0 - 1.0));
    vec4 out_color = vec4(calcColor(normal),1);
	vec4 base_color = useTex? out_color * texture(texSampler, fTex) : useWood? out_color * vec4(woodColor,1) : out_color;
	vec3 I = normalize(fPosition - eye);
    vec3 R = reflect(I, normalize(normal));
    vec4 reflective_color = reflectivity != 0? vec4(texture(skybox, R).rgb, 1.0) : vec4(0,0,0,0);
    fColor = reflectivity * reflective_color + (1 - reflectivity) * base_color;
}
