#pragma once
#include "Rasterizer.h"

enum class RotationAxis {
	X, Y, Z
};

class Camera {
	float mMaxSideLength;
	vec4 mEye;
	vec4 mAt;
	vec4 mUp;
	mat4 mCameraTransform;
	mat4 mLookAt;
	mat4 mProjection;
	mat4 mRotations;

public:
	Camera();
	explicit Camera(const vec4& eye);
	const vec4& getEye();
	const vec4& getAt();
	void transform(const mat4& m); // for scaling
	void transform(const float degrees, const RotationAxis& axis); // for rotating
	void transform(const vec3& v); // for translatin
	float getMaxSideLength();
	mat4 getTransform();
	mat4 getRotations();
	mat4 getProjection();
	void lookAt(const vec4& eye, const vec4& at, const vec4& up);
	void ortho(const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar);
	void frustum(const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar);
	void perspective(const float fovy, const float aspect,
		const float zNear, const float zFar);
	void draw(RasterizerPtr rasterizer, const mat4& from3dTo2d);
};

typedef shared_ptr<Camera> CameraPtr;
