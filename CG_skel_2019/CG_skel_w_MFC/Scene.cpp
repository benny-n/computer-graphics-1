#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>

using namespace std;

// Camera
void Camera::setTransformation(const mat4& transform) {
	cTransform = transform;
}

void Camera::LookAt(const vec4& eye, const vec4& at, const vec4& up) {
	const vec4 n = normalize(eye - at);
	const vec4 u = normalize(cross(up, n));
	const vec4 v = normalize(cross(n, u));
	const vec4 t = vec4(0.0, 0.0, 0.0, 1.0);
	const mat4 c = mat4(u, v, n, t);
	cTransform = c * Translate(-eye) * cTransform;
}

void Camera::Ortho(const float left, const float right,
	const float bottom, const float top,
	const float zNear, const float zFar) {
	mat4 ortho;
	ortho[0][0] = 2 / (right - left);
	ortho[1][1] = 2 / (top - bottom);
	ortho[2][2] = 2 / (zNear - zFar);
	ortho[0][3] = (left + right) / (left - right);
	ortho[1][3] = (bottom + top) / (bottom - top);
	ortho[2][3] = (zNear + zFar) / (zNear - zFar);
	projection = ortho;
}

void Camera::Frustum(const float left, const float right,
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
	projection = frustum;
}

void Camera::Perspective(const float fovy, const float aspect,
	const float zNear, const float zFar) {
	//TODO?
}

// Scene
void Scene::loadOBJModel(string fileName) {
	MeshModel *model = new MeshModel(fileName);
	models.push_back(model);
}

void Scene::draw() {
	//TODO
	// 1. Send the renderer the current camera transform and the projection
	// 2. Tell all models to draw themselves
	PyramidMeshModel Pyramid;
	Pyramid.draw(*m_renderer);
	CubeMeshModel cube;
	//cube.draw(*m_renderer);
	m_renderer->SwapBuffers();
}

void Scene::drawDemo() {
	m_renderer->SetDemoBuffer();
	m_renderer->SwapBuffers();
}
