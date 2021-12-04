#pragma once
#include "vec.h"

struct Material {
	float ka = 0.5; // fraction of ambient light reflected from surface
	float kd = 0.5; // fraction of diffuse light reflected from surface
	float ks = 0.5; // fraction of specular light reflected from surface
	float alpha = 1; // shininess coefficient

	vec3 color = vec3(1, 1, 1);
};
