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
uniform mat4 normalview;
uniform vec3 eye;
in vec3 vPosition;
in vec3 vNormal;
in vec3 ka;
in vec3 kd;
in vec3 ks;
in vec3 emission;
in float alpha;
in vec2 tex;


varying vec4 out_color;
out vec2 fTex;

vec3 calcColor(vec3 position, vec3 normal) {
	vec3 color = vec3(0, 0, 0);
	vec3 v = normalize(eye - position);
	vec3 n = normal;
	vec3 l, r;

	for (int i = 0; i < numLights; i++) {
		switch (lights[i].type) {
			case AMBIENT:
				l = v;
				break;
			case POINT: 
				l = normalize(lights[i].position - position);
				break;
			case PARALLEL: 
				l = normalize(-lights[i].direction);
				break;
			default:
			return vec3(1,0,0);
		}
		r = normalize(2 * n * (dot(n, l)) - l);
		vec3 Ia = ka * lights[i].la;
		vec3 Id = kd * max(0, dot(l, n)) * lights[i].ld;
		vec3 Is = dot(n, l) < 0? vec3(0,0,0) : ks * pow(max(0, dot(r, v)), alpha) * lights[i].ls;
		color = color + Ia + Id + Is;
	}

	return color + emission;
}

void main()
{
	vec3 modifiedPosition = (modelview * vec4(vPosition,1)).xyz;
	vec3 modifiedNormal = (normalview * vec4(vNormal,1)).xyz;
    gl_Position = vec4(modifiedPosition,1);
    out_color = vec4(calcColor(modifiedPosition, modifiedNormal),1);
	fTex = tex;
}
