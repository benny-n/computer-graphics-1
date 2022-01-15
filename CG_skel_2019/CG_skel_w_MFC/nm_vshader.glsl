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
varying vec3 woodColor;
out vec2 fTex;
out mat3 TBN;

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
    woodColor = wood();
    fTex = tex;

    vec3 T = normalize(vec3(normalview * vec4(tangent, 0.0)));
    vec3 B = normalize(vec3(normalview * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(normalview * vec4(0.0, 0.0, 1.0, 1.0)));
    TBN = mat3(T, B, N);
}
