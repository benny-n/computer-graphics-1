#pragma once

#include "vec.h"
#include "mat.h"

using namespace std;

enum class LightType: GLint {
	Ambient = 0, Point = 1, Parallel = 2
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
	virtual vec3 getPosition();
	virtual vec3 getDirection();
};

class AmbientLight : public Light {
public:
	AmbientLight() : Light(Color{0.5,0.5,0.5}, Color{0,0,0}, Color{0,0,0}) {}
	LightType getType() override;
	string getTypeString() override;
};

class PointLight : public Light {
	vec4 mPosition;
public:
	PointLight(const vec3& p) : Light(Color{ 0,0,0 }, Color{ 0.5,0.5,0.5 }, Color{ 0.3,0.3,0.3 }), mPosition(p) {}
	LightType getType() override;
	string getTypeString() override;
	vec3 getPosition() override;
	void setPosition(const vec3& p);
	void translate(const vec3& v) override;
};

class ParallelLight : public Light {
	vec4 mDirection;
public:
	ParallelLight(const vec3& d) : Light(Color{ 0,0,0 }, Color{ 0.5,0.5,0.5 }, Color{ 0.3,0.3,0.3 }), mDirection(d) {}
	LightType getType() override;
	string getTypeString() override;
	vec3 getDirection() override;
	void setDirection(const vec3& d);
	void rotate(const mat4& m) override;
};

typedef shared_ptr<Light> LightPtr;
