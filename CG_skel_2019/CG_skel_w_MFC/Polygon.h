#pragma once

#include "vec.h"

class Triangle {
public:
	vec3 mVertices[3];
	// for refactor
	// vec3 mVertexNormals[3]; 
	// vec3 mFaceNormal;

	int mMinY;
	int mMaxY;

	Triangle(const vec3& a, const vec3& b, const vec3& c);

	bool operator< (const Triangle& t);
};

class Poly {
public:
	Triangle mTriangle;
	Triangle mTriangle3d;

	Poly(const Triangle& t, const Triangle& t3d);
	vec2 span(int y) const;

	bool operator< (const Poly& p);
};

bool polySetCompare(const Poly& a, const Poly& b);