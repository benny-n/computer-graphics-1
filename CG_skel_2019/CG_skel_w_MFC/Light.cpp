#include "stdafx.h"
#include "Light.h"

void Light::setColor(const Color& c) { 
	mLa = c; 
	mLd = c; 
	mLs = c; 
}

void Light::setColor(const Color& c1, const Color& c2, const Color& c3) {
	mLa = c1;
	mLd = c2;
	mLs = c3;
}

void Light::modifyIntensities(const vec3& v) {
	mLa.r *= v.x;
	mLa.g *= v.x;
	mLa.b *= v.x;

	mLd.r *= v.y;
	mLd.g *= v.y;
	mLd.b *= v.y;

	mLs.r *= v.z;
	mLs.g *= v.z;
	mLs.b *= v.z;
}

LightType AmbientLight::getType() { return LightType::Ambient; }

string AmbientLight::getTypeString(){ return "ambient"; }


LightType PointLight::getType() { return LightType::Point; }

string PointLight::getTypeString() { return "point"; }


void PointLight::setPosition(const vec3& p) { mPosition = p; }

void PointLight::translate(const vec3& v) {
	mPosition.x += v.x;
	mPosition.y += v.y;
	mPosition.z += v.z;
}

LightType ParallelLight::getType() { return LightType::Parallel; }

string ParallelLight::getTypeString() { return "parallel"; }


void ParallelLight::setNormal(const vec3& n) { mNormal = n; }

void ParallelLight::rotate(const mat4& m) {
	mNormal = m * mNormal;
}
