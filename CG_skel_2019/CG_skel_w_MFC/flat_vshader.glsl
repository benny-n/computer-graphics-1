#version 150
#define AMBIENT 0
#define POINT 1
#define PARALLEL 2


struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 la;
    vec3 ld;
    vec3 ls;
};

uniform int numLights;
uniform Light lights[];
uniform mat4 modelview;
uniform vec3 eye;
in vec4 vPosition;
in vec3 faceCenter;
in vec3 faceNormal;
in vec3 ka;
in vec3 kd;
in vec3 ks;
in vec3 emission;
in float alpha;

out vec4 out_color;

vec3 calcColor() {
	vec3 color;
	vec3 v = normalize(eye - faceCenter);
	vec3 l, r;

	for (int i = 0; i < numLights; i++) {
		switch (lights[i].type) {
		case AMBIENT: {
			l = v;
			break;
		}
		case POINT: {
			l = normalize(lights[i].position - faceCenter);
			break;
		}
		case PARALLEL: {
			l = normalize(-lights[i].direction);
			break;
		}
		}
		r = normalize(2 * faceNormal * (dot(faceNormal, l)) - l);
		vec3 Ia = ka * lights[i].la;
		vec3 Id = kd * max(0, dot(l, faceNormal)) * lights[i].ld;
		vec3 Is = dot(faceNormal, l) < 0? vec3(0,0,0) : ks * pow(max(0, dot(r, v)), alpha) * lights[i].ls;
		color = color + Ia + Id + Is;
	}

	return color + emission;
}

void main()
{
    gl_Position = modelview * vPosition;
    out_color = vec4(calcColor(),1);
}
