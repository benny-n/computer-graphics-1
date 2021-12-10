#pragma once

#include "Color.h"
#include "Material.h"
#include "vec.h"

class Triangle {
public:
	vec3 mVertices[3];
	vec3 mVertexNormals[3];

	Triangle(const vec3 v[3]);
	Triangle(const vec3 v[3], const vec3 n[3]);
	vec3 center() const;
	virtual vec3 getFaceNormal() const;
};

class ProjectedTriangle : public Triangle {
public:
	ProjectedTriangle(const vec3 v[3]) : Triangle(v) {}
	vec3 getFaceNormal() const override;
};

class ScreenTriangle {
public:
	vec2 mVertices[3];

	ScreenTriangle(const vec2 v[3]);
};

class Poly {
public:
	Poly(const Triangle& t, const ProjectedTriangle& pt, const ScreenTriangle& st,
		const Material m[3], bool drawFaceNormal, bool drawVertexNormal);
	Triangle mTriangle;
	ProjectedTriangle mProjectedTriangle;
	ScreenTriangle mScreenTriangle;
	Material mMaterial;
	Color mVertexColors[3];
	bool mDrawFaceNormal;
	bool mDrawVertexNormal;
	int mMinY;
	int mMaxY;

	bool operator< (const Poly& p);
	vec2 span(int y) const;
	vec3 barycentricCoordinates(int x, int y) const;
};

struct TriangleSetComparator {
	bool operator()(const Triangle& a, const Triangle& b) const;
};

struct PolySetComparator {
	bool operator()(const Poly& a, const Poly& b) const;
};