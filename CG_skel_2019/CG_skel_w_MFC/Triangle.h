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

	Triangle(const vec3 v[3]);
	Triangle(const vec3& a, const vec3& b, const vec3& c);
	vec2 span(int y) const;

	bool operator< (const Triangle& t);
};

struct TriangleSetComparator {
	bool operator()(const Triangle& a, const Triangle& b) const;
};