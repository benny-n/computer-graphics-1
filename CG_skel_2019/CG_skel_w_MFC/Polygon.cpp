#include "stdafx.h"
#include "Polygon.h"

Triangle::Triangle(const vec3 v[3]) : Triangle(v[0], v[1], v[2]) {}

Triangle::Triangle(const vec3& a, const vec3& b, const vec3& c) {
	mVertices[0] = a;
	mVertices[1] = b;
	mVertices[2] = c;

	mMinY = MIN3(a.y, b.y, c.y);
	mMaxY = MAX3(a.y, b.y, c.y);
}

bool Triangle::operator<(const Triangle& t) { return mMinY < t.mMinY; }

Poly::Poly(const Triangle& t, const Triangle& t3d) : mTriangle(t), mTriangle3d(t3d) {}

vec2 Poly::span(int y) const {
	vec2 p1(mTriangle.mVertices[0].x, mTriangle.mVertices[0].y);
	vec2 p2(mTriangle.mVertices[1].x, mTriangle.mVertices[1].y);
	vec2 p3(mTriangle.mVertices[2].x, mTriangle.mVertices[2].y);

	float xMin = FLT_MAX;
	float xMax = FLT_MIN;

	auto intersect = [&xMin, &xMax](const vec2& p1, const vec2& p2, int y) {
		if (p1.y == p2.y) {
			if (p1.y == y) {
				xMin = MIN3(xMin, p1.x, p2.x);
				xMax = MAX3(xMax, p1.x, p2.x);
			}
		}
		else {
			float alpha = (y - p1.y) / (p2.y - p1.y);
			if (0 <= alpha && alpha <= 1) {
				float x = (1 - alpha) * p1.x + alpha * p2.x;
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

bool Poly::operator<(const Poly& p) { return mTriangle < p.mTriangle; }

bool PolySetComparator::operator()(const Poly& a, const Poly& b) const {
	return (a.mTriangle3d.mVertices[0] < b.mTriangle3d.mVertices[0]) 
		|| (a.mTriangle3d.mVertices[0] == b.mTriangle3d.mVertices[0]) && (a.mTriangle3d.mVertices[1] < b.mTriangle3d.mVertices[1]) 
		|| (a.mTriangle3d.mVertices[0] == b.mTriangle3d.mVertices[0]) && (a.mTriangle3d.mVertices[1] == b.mTriangle3d.mVertices[1]) 
			&& (a.mTriangle3d.mVertices[2] < b.mTriangle3d.mVertices[2]);
}
