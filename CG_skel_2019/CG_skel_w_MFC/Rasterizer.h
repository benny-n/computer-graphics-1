#pragma once

#include "vec.h"
#include "Color.h"
#include "Polygon.h"
#include "Light.h"

class Rasterizer {
protected:
	const vector<LightPtr>& mLights;
public:
	Rasterizer(const vector<LightPtr>& lights) : mLights(lights) {}

	Color calcColorForPoint(const Poly& polygon, const vec3& eye, const vec3& p, const vec3& n);
	virtual void preprocess(Poly& polygon, const vec3& eye) = 0;
	virtual Color process(int x, int y, const Poly& polygon, const vec3& eye) = 0;
};

class FlatRasterizer : public Rasterizer {
public:
	FlatRasterizer(const vector<LightPtr>& lights) : Rasterizer(lights) {}

	void preprocess(Poly& polygon, const vec3& eye) override;
	Color process(int x, int y, const Poly& polygon, const vec3& eye) override;
};

class GouraudRasterizer : public Rasterizer {
public:
	GouraudRasterizer(const vector<LightPtr>& lights) : Rasterizer(lights) {}

	void preprocess(Poly& polygon, const vec3& eye) override;
	Color process(int x, int y, const Poly& polygon, const vec3& eye) override;
};

class PhongRasterizer : public Rasterizer {
public:
	PhongRasterizer(const vector<LightPtr>& lights) : Rasterizer(lights) {}

	void preprocess(Poly& polygon, const vec3& eye) override {}
	Color process(int x, int y, const Poly& polygon, const vec3& eye) override;
};


typedef unique_ptr<Rasterizer> RasterizerPtr;

