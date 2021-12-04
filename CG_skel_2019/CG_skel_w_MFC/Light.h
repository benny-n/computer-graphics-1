#pragma once

#include "vec.h"

using namespace std;

enum class LightType {
	Ambient, Point, Parallel
};

class Light {
protected:
	~Light() {}
public:
	float mIntensity;
	vec3 mColor;

	Light() : mIntensity(1), mColor(1) {}
	void setColor(const vec3& c);
	virtual LightType getType() = 0;
	virtual string getTypeString() = 0;
};

class AmbientLight : public Light {
public:
	LightType getType() override;
	string getTypeString() override;
};

class PointLight : public Light {
	vec4 mPosition;
public:
	PointLight(const vec3& p) : mPosition(p) {}
	LightType getType() override;
	string getTypeString() override;
	void setPosition(const vec3& p);
};

class ParallelLight : public Light {
	vec4 mNormal;
public:
	ParallelLight(const vec3& n) : mNormal(n) {}
	LightType getType() override;
	string getTypeString() override;
	void setNormal(const vec3& n);
};

typedef shared_ptr<Light> LightPtr;
