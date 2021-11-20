#include "stdafx.h"
#include "Camera.h"

// Camera
Camera::Camera() {
	lookAt(vec4(vec3(0, 0, 10)), vec4(vec3(0, 0, 0)), vec4(vec3(0, 1, 0)));
	ortho(-10, 10, -10, 10, 5, 15);
}

Camera::Camera(const vec4& eye) {
	lookAt(eye, vec4(vec3()), vec4(vec3(0, 1, 0)));
	ortho(-10, 10, -10, 10, 5, 15);
}

const vec4& Camera::getEye()
{
	return mEye;
}

const vec4& Camera::getAt()
{
	return mAt;
}

void Camera::transform(const mat4& m) {
	//const mat4 transformation = Translate(eye) * m * Translate(-eye);
	//cTransform = transformation * cTransform;
	// TODO might need above code, try zooming when not looking at origin
	mCameraTransform = m * mCameraTransform;
}

void Camera::transform(const float degrees, const RotationAxis& axis) {
	mat4 rotation;
	switch (axis) {
	case RotationAxis::X:
		rotation = rotateX(degrees);
		break;
	case RotationAxis::Y:
		rotation = rotateY(degrees);
		break;
	case RotationAxis::Z:
		rotation = rotateZ(degrees);
		break;
	}
	// We transform from normal axes to the camera axes, then 
	// we apply the rotation matrix to 'at' and 'up', then
	// we transform back to normal axes and use LookAt
	lookAt(mEye, (rotation * (mAt - mEye)) + mEye, rotation * mUp);
}

void Camera::transform(const vec3& v) {
	lookAt(mEye + vec4(v, 0), mAt + vec4(v, 0), mUp);
}

mat4 Camera::getTransform() { return mCameraTransform * mLookAt; }
mat4 Camera::getProjection() { return mProjection; }

void Camera::lookAt(const vec4& eye, const vec4& at, const vec4& up) {
	mEye = eye;
	mAt = at;
	mUp = up;
	const vec4 n = normalize(eye - at);
	const vec4 u = vec4(normalize(cross(up, n)), 0);
	const vec4 v = vec4(normalize(cross(n, u)), 0);
	const vec4 t = vec4(0.0, 0.0, 0.0, 1.0);
	const mat4 c = mat4(u, v, n, t);
	mLookAt = c * translate(-eye);
}

void Camera::ortho(const float left, const float right,
	const float bottom, const float top,
	const float zNear, const float zFar) {
	mat4 ortho;
	ortho[0][0] = 2 / (right - left);
	ortho[1][1] = 2 / (top - bottom);
	ortho[2][2] = 2 / (zNear - zFar);
	ortho[0][3] = (left + right) / (left - right);
	ortho[1][3] = (bottom + top) / (bottom - top);
	ortho[2][3] = (zNear + zFar) / (zNear - zFar);
	mProjection = ortho;
}

void Camera::frustum(const float left, const float right,
	const float bottom, const float top,
	const float zNear, const float zFar) {
	mat4 frustum;
	frustum[0][0] = (2 * zNear) / (right - left);
	frustum[1][1] = (2 * zNear) / (top - bottom);
	frustum[2][2] = (zNear + zFar) / (zNear - zFar);
	frustum[3][3] = 0;
	frustum[0][2] = (right + left) / (right - left);
	frustum[1][2] = (top + bottom) / (top - bottom);
	frustum[3][2] = -1;
	frustum[2][3] = (2 * zNear * zFar) / (zNear - zFar);
	mProjection = frustum;
}

void Camera::perspective(const float fovy, const float aspect,
	const float zNear, const float zFar) {
	float top = zNear * std::tan((fovy) / 2);
	float bottom = -top;
	float right = top * aspect;
	float left = -right;
	frustum(left, right, bottom, top, zNear, zFar);
}

void Camera::draw(Renderer* renderer) {
	renderer->drawCamera(mEye);
}
