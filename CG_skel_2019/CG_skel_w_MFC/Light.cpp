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
	mLa.r *= v[0];
	mLa.g *= v[0];
	mLa.b *= v[0];

	mLd.r *= v[1];
	mLd.g *= v[1];
	mLd.b *= v[1];

	mLs.r *= v[2];
	mLs.g *= v[2];
	mLs.b *= v[2];
}

LightType AmbientLight::getType() { return LightType::Ambient; }

string AmbientLight::getTypeString(){ return "ambient"; }


LightType PointLight::getType() { return LightType::Point; }

string PointLight::getTypeString() { return "point"; }

vec3 PointLight::getPosition()
{
	return vec3FromVec4(mPosition);
}


void PointLight::setPosition(const vec3& p) { mPosition = p; }

void PointLight::translate(const vec3& v) {
	mPosition.x += v.x;
	mPosition.y += v.y;
	mPosition.z += v.z;
}

LightType ParallelLight::getType() { return LightType::Parallel; }

string ParallelLight::getTypeString() { return "parallel"; }

vec3 ParallelLight::getDirection()
{
	return vec3FromVec4(mDirection);
}


void ParallelLight::setDirection(const vec3& d) { mDirection = d; }

void ParallelLight::rotate(const mat4& m) {
	mDirection = m * mDirection;
}
