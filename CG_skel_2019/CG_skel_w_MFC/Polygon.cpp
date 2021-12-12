#include "stdafx.h"
#include "Polygon.h"

Triangle::Triangle(const vec3 v[3]) {
	mVertices[0] = v[0];
	mVertices[1] = v[1];
	mVertices[2] = v[2];
}

Triangle::Triangle(const vec3 v[3], const vec3 n[3]) {
	mVertices[0] = v[0];
	mVertices[1] = v[1];
	mVertices[2] = v[2];

	mVertexNormals[0] = n[0];
	mVertexNormals[1] = n[1];
	mVertexNormals[2] = n[2];
}

vec3 Triangle::center() const { return (mVertices[0] + mVertices[1] + mVertices[2]) / 3; }

vec3 Triangle::getFaceNormal() const {
	vec3 i = mVertices[1] - mVertices[0];
	vec3 j = mVertices[2] - mVertices[0];
	return vec3(normalize(cross(i, j)));
}

vec3 ProjectedTriangle::getFaceNormal() const {
	vec3 i = mVertices[1] - mVertices[0];
	vec3 j = mVertices[2] - mVertices[0];
	return vec3(normalize(cross(j, i))); // swap i, j because triangle gets projected vertices that are flipped by camera POV
}

ScreenTriangle::ScreenTriangle(const vec2 v[3]) {
	mVertices[0] = v[0];
	mVertices[1] = v[1];
	mVertices[2] = v[2];
}

Poly::Poly(const Triangle& t, const ProjectedTriangle& pt, const ScreenTriangle& st,
	const Material m[3], bool drawFaceNormal, bool drawVertexNormal) :
	mTriangle(t), mProjectedTriangle(pt), mScreenTriangle(st), mDrawFaceNormal(drawFaceNormal), mDrawVertexNormal(drawVertexNormal) {
	mMaterial = (m[0] + m[1] + m[2]) / 3;

	mMinY = MIN3(mScreenTriangle.mVertices[0].y, mScreenTriangle.mVertices[1].y, mScreenTriangle.mVertices[2].y);
	mMaxY = MAX3(mScreenTriangle.mVertices[0].y, mScreenTriangle.mVertices[1].y, mScreenTriangle.mVertices[2].y);
	mMinX = MIN3(mScreenTriangle.mVertices[0].x, mScreenTriangle.mVertices[1].x, mScreenTriangle.mVertices[2].x);
	mMaxX = MAX3(mScreenTriangle.mVertices[0].x, mScreenTriangle.mVertices[1].x, mScreenTriangle.mVertices[2].x);
}

bool Poly::operator<(const Poly& p) { return mMinY < p.mMinY; }

vec2 Poly::span(int y) const {
	const vec2& p1 = mScreenTriangle.mVertices[0];
	const vec2& p2 = mScreenTriangle.mVertices[1];
	const vec2& p3 = mScreenTriangle.mVertices[2];

	float xMin = FLT_MAX;
	float xMax = FLT_MIN;

	const auto intersect = [&xMin, &xMax](const vec2& p1, const vec2& p2, int y) {
		if (p1.y == p2.y) {
			if (p1.y == y) {
				xMin = MIN3(xMin, p1.x, p2.x);
				xMax = MAX3(xMax, p1.x, p2.x);
			}
		}
		else {
			const float alpha = (y - p1.y) / (p2.y - p1.y);
			if (0 <= alpha && alpha <= 1) {
				const float x = (1 - alpha) * p1.x + alpha * p2.x;
				xMin = min(xMin, x);
				xMax = max(xMax, x);
			}
		}
	};

	intersect(p1, p2, y);
	intersect(p2, p3, y);
	intersect(p3, p1, y);

	return vec2(xMin, xMax);
}

inline static float calcTriangleSurface(const vec2& p1, const vec2& p2, const vec2& p3) {
	vec2 i = p2 - p1;
	vec2 j = p3 - p1;
	return abs((i.x * j.y - i.y * j.x) / 2);
}

vec3 Poly::barycentricCoordinates(int x, int y)  const {
	vec2 p(x, y);
	float A = calcTriangleSurface(mScreenTriangle.mVertices[0], mScreenTriangle.mVertices[1], mScreenTriangle.mVertices[2]);
	float A0 = calcTriangleSurface(p, mScreenTriangle.mVertices[1], mScreenTriangle.mVertices[2]);
	float A1 = calcTriangleSurface(mScreenTriangle.mVertices[0], p, mScreenTriangle.mVertices[2]);
	float A2 = calcTriangleSurface(mScreenTriangle.mVertices[0], mScreenTriangle.mVertices[1],p);
	return vec3(A0, A1, A2) / A;
}

bool TriangleSetComparator::operator()(const Triangle& a, const Triangle& b) const {
	return (a.mVertices[0] < b.mVertices[0]) 
		|| (a.mVertices[0] == b.mVertices[0]) && (a.mVertices[1] < b.mVertices[1]) 
		|| (a.mVertices[0] == b.mVertices[0]) && (a.mVertices[1] == b.mVertices[1]) && (a.mVertices[2] < b.mVertices[2]);
}

bool PolySetComparator::operator()(const Poly& a, const Poly& b) const
{
	const TriangleSetComparator tsc;
	return tsc(a.mTriangle, b.mTriangle);
}
