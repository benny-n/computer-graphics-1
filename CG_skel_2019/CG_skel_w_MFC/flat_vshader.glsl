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

float noise(float x, float y) {
	int ix = int(x);
	int iy = int(y);
    int n;
    n = ix+iy*57;
    x = (n<<13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float turbulence(float x, float y) {
    float corners, sides, center;
    corners = (noise(x-1, y-1) + noise(x+1, y-1) + noise(x-1, y+1) + noise(x+1, y+1) ) / 16;
    sides   = (noise(x-1, y) + noise(x+1, y) + noise(x, y-1) + noise(x, y+1) ) /  8;
    center  =  noise(x, y) / 4;
    return corners + sides + center;
}

vec3 wood_color(float x) {
	vec3 c1 = vec3(0.0,0.0,0.0);
	vec3 c2 = vec3(0.64, 0.45,0.28);
	float t = (sin(x) + 1) / 2;
	return t * c1 + ( 1-t ) * c2;
}

vec3 wood() {
	float noisy = pow(vPosition.x,2) + pow(vPosition.y,2) + turbulence(vPosition.x, vPosition.y);
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
