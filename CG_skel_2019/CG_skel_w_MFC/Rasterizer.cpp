#include "stdafx.h"
#include "Rasterizer.h"
#include <math.h>

Color Rasterizer::calcColorForPoint(const Poly& polygon, const vec3& eye, const vec3& p, const vec3& n) {
	Color color{ 0,0,0 };
	vec3 v = normalize(eye - p);
	vec3 l, r;

	for each (auto light in mLights) {
		switch (light->getType()) {
		case LightType::Ambient: {
			l = v;
			break;
		}
		case LightType::Point: {
			auto pl = dynamic_pointer_cast<PointLight>(light);
			l = normalize(pl->getPosition() - p);
			break;
		}
		case LightType::Parallel: {
			auto pl = dynamic_pointer_cast<ParallelLight>(light);
			l = normalize(-pl->getDirection());
			break;
		}
		}
		r = normalize(2 * n * (dot(n, l)) - l);
		auto Ia = polygon.mMaterial.ka * light->mLa;
		auto Id = polygon.mMaterial.kd * max(0, dot(l, n)) * light->mLd;
		auto Is = dot(n, l) < 0? Color{ 0,0,0 } : polygon.mMaterial.ks * pow(max(0, dot(r, v)), polygon.mMaterial.alpha) * light->mLs;
		color = color + Ia + Id + Is;
	}

	return color + polygon.mMaterial.emission;
}

void FlatRasterizer::preprocess(Poly& polygon, const vec3& eye) {
	vec3 p = polygon.mTriangle.center();
	vec3 n = polygon.mTriangle.getFaceNormal();
	Color color = calcColorForPoint(polygon, eye, p, n);
	for (int i = 0; i < 3; i++)
		polygon.mVertexColors[i] = color;
}

Color FlatRasterizer::process(int x, int y, const Poly& polygon, const vec3& eye) { return polygon.mVertexColors[0]; }

void GouraudRasterizer::preprocess(Poly& polygon, const vec3& eye) {
	for (int i = 0; i < 3; i++) {
		vec3 p = polygon.mTriangle.mVertexPositions[i];
		vec3 n = polygon.mTriangle.mVertexNormals[i];
		polygon.mVertexColors[i] = calcColorForPoint(polygon, eye, p, n);
	}
}

Color GouraudRasterizer::process(int x, int y, const Poly& polygon, const vec3& eye) {
	vec3 bc = polygon.barycentricCoordinates(x, y);
	return polygon.mVertexColors[0] * bc[0] + polygon.mVertexColors[1] * bc[1] + polygon.mVertexColors[2] * bc[2];
}

Color PhongRasterizer::process(int x, int y, const Poly& polygon, const vec3& eye) {
	vec3 bc = polygon.barycentricCoordinates(x, y);
	// reverse linear interpolation
	vec3 p = polygon.mTriangle.mVertexPositions[0] * bc[0] + polygon.mTriangle.mVertexPositions[1] * bc[1] + polygon.mTriangle.mVertexPositions[2] * bc[2];
	vec3 n = polygon.mTriangle.mVertexNormals[0] * bc[0] + polygon.mTriangle.mVertexNormals[1] * bc[1] + polygon.mTriangle.mVertexNormals[2] * bc[2];
	return calcColorForPoint(polygon, eye, p, n);
}

