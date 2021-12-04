#pragma once
#include "Color.h"

struct Material {
	Color ka {0.5, 0.5, 0.5}; // fraction of ambient light reflected from surface
	Color kd {0.5, 0.5, 0.5}; // fraction of diffuse light reflected from surface
	Color ks {0.5, 0.5, 0.5}; // fraction of specular light reflected from surface
	float alpha = 0.5; // shininess coefficient
};
