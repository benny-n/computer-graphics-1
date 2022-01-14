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
uniform sampler2D texSampler;
uniform sampler2D normalMap;

varying vec2 fTex;
varying vec3 fPosition;
in mat3 TBN;
in float fAlpha;

out vec4 fColor;

vec3 calcColor() {
	vec3 color = vec3(0, 0, 0);
	vec3 v = normalize(eye - fPosition);
	vec3 n = normalize(TBN * (texture(normalMap, fTex).rgb * 2.0 - 1.0));
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
		vec3 Ia = lights[i].la;
		vec3 Id = max(0, dot(l, n)) * lights[i].ld;
		vec3 Is = dot(n, l) < 0? vec3(0,0,0) : pow(max(0, dot(r, v)), fAlpha) * lights[i].ls;
		color = color + Ia + Id + Is;
	}

	return color;
}

void main() {
    vec4 out_color = vec4(calcColor(),1);
	fColor = out_color * texture(texSampler, fTex);
}
