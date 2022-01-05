#include "stdafx.h"
#include "Polygon.h"

Triangle::Triangle(const vec3 v[3]) {
	mVertexPositions[0] = v[0];
	mVertexPositions[1] = v[1];
	mVertexPositions[2] = v[2];
}

Triangle::Triangle(const vec3 v[3], const vec3 n[3]) {
	mVertexPositions[0] = v[0];
	mVertexPositions[1] = v[1];
	mVertexPositions[2] = v[2];

	mVertexNormals[0] = n[0];
	mVertexNormals[1] = n[1];
	mVertexNormals[2] = n[2];
}

vec3 Triangle::center() const { return (mVertexPositions[0] + mVertexPositions[1] + mVertexPositions[2]) / 3; }

vec3 Triangle::getFaceNormal() const {
	vec3 i = mVertexPositions[1] - mVertexPositions[0];
	vec3 j = mVertexPositions[2] - mVertexPositions[0];
	return vec3(normalize(cross(i, j)));
}

vec3 ProjectedTriangle::getFaceNormal() const {
	vec3 i = mVertexPositions[1] - mVertexPositions[0];
	vec3 j = mVertexPositions[2] - mVertexPositions[0];
	return vec3(normalize(cross(j, i))); // swap i, j because triangle gets projected vertices that are flipped by camera POV
}

ScreenTriangle::ScreenTriangle(const vec2 v[3]) {
	mVertexPositions[0] = v[0];
	mVertexPositions[1] = v[1];
	mVertexPositions[2] = v[2];
}

Poly::Poly(const Triangle& t, const ProjectedTriangle& pt, const ScreenTriangle& st,
	const Material m[3], bool drawFaceNormal, bool drawVertexNormal) :
	mTriangle(t), mProjectedTriangle(pt), mScreenTriangle(st), mDrawFaceNormal(drawFaceNormal), mDrawVertexNormal(drawVertexNormal) {
	mMaterial = (m[0] + m[1] + m[2]) / 3;

	mMinY = MIN3(mScreenTriangle.mVertexPositions[0].y, mScreenTriangle.mVertexPositions[1].y, mScreenTriangle.mVertexPositions[2].y);
	mMaxY = MAX3(mScreenTriangle.mVertexPositions[0].y, mScreenTriangle.mVertexPositions[1].y, mScreenTriangle.mVertexPositions[2].y);
	mMinX = MIN3(mScreenTriangle.mVertexPositions[0].x, mScreenTriangle.mVertexPositions[1].x, mScreenTriangle.mVertexPositions[2].x);
	mMaxX = MAX3(mScreenTriangle.mVertexPositions[0].x, mScreenTriangle.mVertexPositions[1].x, mScreenTriangle.mVertexPositions[2].x);
}

bool Poly::operator<(const Poly& p) { return mMinY < p.mMinY; }

vec2 Poly::span(int y) const {
	const vec2& p1 = mScreenTriangle.mVertexPositions[0];
	const vec2& p2 = mScreenTriangle.mVertexPositions[1];
	const vec2& p3 = mScreenTriangle.mVertexPositions[2];

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
	float A = calcTriangleSurface(mScreenTriangle.mVertexPositions[0], mScreenTriangle.mVertexPositions[1], mScreenTriangle.mVertexPositions[2]);
	float A0 = calcTriangleSurface(p, mScreenTriangle.mVertexPositions[1], mScreenTriangle.mVertexPositions[2]);
	float A1 = calcTriangleSurface(mScreenTriangle.mVertexPositions[0], p, mScreenTriangle.mVertexPositions[2]);
	float A2 = calcTriangleSurface(mScreenTriangle.mVertexPositions[0], mScreenTriangle.mVertexPositions[1],p);
	return vec3(A0, A1, A2) / A;
}

bool TriangleSetComparator::operator()(const Triangle& a, const Triangle& b) const {
	return (a.mVertexPositions[0] < b.mVertexPositions[0]) 
		|| (a.mVertexPositions[0] == b.mVertexPositions[0]) && (a.mVertexPositions[1] < b.mVertexPositions[1]) 
		|| (a.mVertexPositions[0] == b.mVertexPositions[0]) && (a.mVertexPositions[1] == b.mVertexPositions[1]) && (a.mVertexPositions[2] < b.mVertexPositions[2]);
}

bool PolySetComparator::operator()(const Poly& a, const Poly& b) const
{
	const TriangleSetComparator tsc;
	return tsc(a.mTriangle, b.mTriangle);
}
