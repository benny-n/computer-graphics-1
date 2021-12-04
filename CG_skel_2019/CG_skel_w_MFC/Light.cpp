#include "stdafx.h"
#include "Light.h"

void Light::setColor(const vec3& c) { mColor = c; }

LightType AmbientLight::getType() { return LightType::Ambient; }

string AmbientLight::getTypeString(){ return "ambient"; }


LightType PointLight::getType() { return LightType::Point; }

string PointLight::getTypeString() { return "point"; }


void PointLight::setPosition(const vec3& p) { mPosition = p; }

LightType ParallelLight::getType() { return LightType::Parallel; }

string ParallelLight::getTypeString() { return "parallel"; }


void ParallelLight::setNormal(const vec3& n) { mNormal = n; }
