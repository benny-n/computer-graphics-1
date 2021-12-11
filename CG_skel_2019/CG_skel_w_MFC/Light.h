#pragma once

#include "vec.h"
#include "mat.h"

using namespace std;

enum class LightType {
	Ambient, Point, Parallel
};

class Light {
protected:
	~Light() {}
public:
	Color mLa;
	Color mLd;
	Color mLs;

	Light() : mLa(Color{0.5, 0.5, 0.5}), mLd(Color{ 0.5, 0.5, 0.5 }), mLs(Color{ 0.5, 0.5, 0.5 }) {}
	Light(Color la, Color ld, Color ls) : mLa(la), mLd(ld), mLs(ls) {}
	void setColor(const Color& c);
	void setColor(const Color& c1, const Color& c2, const Color& c3);
	void modifyIntensities(const vec3& v);
	virtual void translate(const vec3& v) {}
	virtual void rotate(const mat4& m) {}
	virtual LightType getType() = 0;
	virtual string getTypeString() = 0;
};

class AmbientLight : public Light {
public:
	AmbientLight() : Light(Color{0.5,0.5,0.5}, Color{0.15,0.15,0.15}, Color{0.1,0.1,0.1}) {}
	LightType getType() override;
	string getTypeString() override;
};

class PointLight : public Light {
	vec4 mPosition;
public:
	PointLight(const vec3& p) : Light(Color{ 0.1,0.1,0.1 }, Color{ 0.5,0.5,0.5 }, Color{ 0.3,0.3,0.3 }), mPosition(p) {}
	LightType getType() override;
	string getTypeString() override;
	vec3 getPosition();
	void setPosition(const vec3& p);
	void translate(const vec3& v) override;
};

class ParallelLight : public Light {
	vec4 mDirection;
public:
	ParallelLight(const vec3& d) : Light(Color{ 0.1,0.1,0.1 }, Color{ 0.5,0.5,0.5 }, Color{ 0.3,0.3,0.3 }), mDirection(d) {}
	LightType getType() override;
	string getTypeString() override;
	vec3 getDirection();
	void setDirection(const vec3& d);
	void rotate(const mat4& m) override;
};

typedef shared_ptr<Light> LightPtr;
