#pragma once

#include "vec.h"

class Triangle {
public:
	vec3 mVertices[3];
	vec3 mVertexNormals[3];
	bool mDrawFaceNormal;
	bool mDrawVertexNormal;

	int mMinY;
	int mMaxY;

	Triangle(const vec3 v[3], bool drawFaceNormal);
	Triangle(const vec3 v[3], bool drawFaceNormal, const vec3 n[3]);
	void setYMinAndYMax();
	vec2 span(int y) const;
	vec3 center() const;
	vec3 getFaceNormal() const;

	bool operator< (const Triangle& t);
};

struct TriangleSetComparator {
	bool operator()(const Triangle& a, const Triangle& b) const;
};