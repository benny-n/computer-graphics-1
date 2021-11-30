#include "stdafx.h"
#include "Triangle.h"

Triangle::Triangle(const vec3 v[3], bool drawFaceNormal) : mDrawFaceNormal(drawFaceNormal), mDrawVertexNormal(false) {
	mVertices[0] = v[0];
	mVertices[1] = v[1];
	mVertices[2] = v[2];
}

Triangle::Triangle(const vec3 v[3], bool drawFaceNormal, const vec3 n[3]) : mDrawFaceNormal(drawFaceNormal), mDrawVertexNormal(true) {
	mVertices[0] = v[0];
	mVertices[1] = v[1];
	mVertices[2] = v[2];

	mVertexNormals[0] = n[0];
	mVertexNormals[1] = n[1];
	mVertexNormals[2] = n[2];
}

void Triangle::setYMinAndYMax() {
	mMinY = MIN3(mVertices[0].y, mVertices[1].y, mVertices[2].y);
	mMaxY = MAX3(mVertices[0].y, mVertices[1].y, mVertices[2].y);
}

vec2 Triangle::span(int y) const {
	vec2 p1(mVertices[0].x, mVertices[0].y);
	vec2 p2(mVertices[1].x, mVertices[1].y);
	vec2 p3(mVertices[2].x, mVertices[2].y);

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

vec3 Triangle::center() const { return (mVertices[0] + mVertices[1] + mVertices[2]) / 3; }

vec3 Triangle::getFaceNormal() const {
	vec3 i = mVertices[1] - mVertices[0];
	vec3 j = mVertices[2] - mVertices[0];
	return vec3(normalize(cross(j, i))); // swap i, j because triangle gets projected vertices that are flipped by camera POV
}

bool Triangle::operator<(const Triangle& t) { return mMinY < t.mMinY; }

bool TriangleSetComparator::operator()(const Triangle& a, const Triangle& b) const {
	return (a.mVertices[0] < b.mVertices[0]) 
		|| (a.mVertices[0] == b.mVertices[0]) && (a.mVertices[1] < b.mVertices[1]) 
		|| (a.mVertices[0] == b.mVertices[0]) && (a.mVertices[1] == b.mVertices[1]) && (a.mVertices[2] < b.mVertices[2]);
}
