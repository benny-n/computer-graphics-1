#pragma once
#include "Color.h"

struct Material {
	Color ka { 0.5, 0.5, 0.5 }; // fraction of ambient light reflected from surface
	Color kd { 0.5, 0.5, 0.5 }; // fraction of diffuse light reflected from surface
	Color ks { 0.5, 0.5, 0.5 }; // fraction of specular light reflected from surface
	Color emission{ 0, 0, 0 };  // light emission by material
	float alpha = 32; // shininess coefficient

	Material invert() const {
		return Material{
			Color(1,1,1) - ka,
			Color(1,1,1) - kd,
			Color(1,1,1) - ks,
			emission, alpha
		};
	}

	Material operator + (const Material& m) const {
		return Material{ ka + m.ka, kd + m.kd, ks + m.ks, emission + m.emission, alpha + m.alpha };
	}

	Material operator / (const float s) const {
		return Material{ ka / s, kd / s, ks / s, emission / s ,alpha / s };
	}
};
