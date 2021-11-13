#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>

using namespace std;

//Boundry Box

void Model::BoundryBox::initVertexPositions()
{
	// First Face (bottom)
	vertex_positions[0] = vec3(vmin.x, vmin.y, vmax.z);
	vertex_positions[1] = vec3(vmin.x, vmin.y, vmin.z);
	vertex_positions[2] = vec3(vmax.x, vmin.y, vmin.z);
	vertex_positions[3] = vec3(vmax.x, vmin.y, vmax.z);
	// Second Face (front)
	vertex_positions[4] = vec3(vmax.x, vmin.y, vmax.z);
	vertex_positions[5] = vec3(vmin.x, vmin.y, vmax.z);
	vertex_positions[6] = vec3(vmin.x, vmax.y, vmax.z);
	vertex_positions[7] = vec3(vmax.x, vmax.y, vmax.z);
	// Third Face (top)
	vertex_positions[8] = vec3(vmax.x, vmax.y, vmax.z);
	vertex_positions[9] = vec3(vmin.x, vmax.y, vmax.z);
	vertex_positions[10] = vec3(vmin.x, vmax.y, vmin.z);
	vertex_positions[11] = vec3(vmax.x, vmax.y, vmin.z);
	// Fourth Face (back)
	vertex_positions[12] = vec3(vmax.x, vmax.y, vmin.z);
	vertex_positions[13] = vec3(vmax.x, vmin.y, vmin.z);
	vertex_positions[14] = vec3(vmin.x, vmin.y, vmin.z);
	vertex_positions[15] = vec3(vmin.x, vmax.y, vmin.z);
	// Fifth Face (left)
	vertex_positions[16] = vec3(vmin.x, vmax.y, vmin.z);
	vertex_positions[17] = vec3(vmin.x, vmin.y, vmin.z);
	vertex_positions[18] = vec3(vmin.x, vmin.y, vmax.z);
	vertex_positions[19] = vec3(vmin.x, vmax.y, vmax.z);
	// Sixth Face (right)
	vertex_positions[20] = vec3(vmax.x, vmax.y, vmax.z);
	vertex_positions[21] = vec3(vmax.x, vmax.y, vmin.z);
	vertex_positions[22] = vec3(vmax.x, vmin.y, vmin.z);
	vertex_positions[23] = vec3(vmax.x, vmax.y, vmin.z);
}

void Model::BoundryBox::transform(const mat4& m)
{
	vmin = m * vmin;
	vmax = m * vmax;
}

vec4 Model::BoundryBox::center(){
	return (vmin + vmax) / 2.0;
}

void Model::BoundryBox::draw(Renderer& renderer){
	renderer.SetColor(vec3(0.5));
	renderer.DrawSquares(&vertex_positions);
}

// Model
const string& Model::getName()
{
	return name;
}

// Camera
void Camera::setTransformation(const mat4& transform) {
	cTransform = transform;
}

mat4 Camera::getTransform() { return cTransform; }
mat4 Camera::getProjection() { return projection; }

void Camera::LookAt(const vec4& eye, const vec4& at, const vec4& up) {
	const vec4 n = normalize(eye - at);
	const vec4 u = vec4(normalize(cross(up, n)),0);
	const vec4 v = vec4(normalize(cross(n, u)),0);
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
	float top = zNear * std::tan((fovy) / 2);
	float bottom = -top;
	float right = top * aspect;
	float left = -right;
	Frustum(left, right, bottom, top, zNear, zFar);
}

// Scene
Scene::Scene() : m_renderer(), activeModel(0), activeLight(0), activeCamera(0) {
	auto default_camera = make_shared<Camera>();
	cameras.push_back(default_camera);
}

Scene::Scene(Renderer* renderer) : m_renderer(renderer), activeModel(0), activeLight(0), activeCamera(0) {
	auto default_camera = make_shared<Camera>();
	cameras.push_back(default_camera);
}

const vector<ModelPtr>& Scene::getModels()
{
	return models;
}

void Scene::loadOBJModel(string fileName) {
	auto model = make_shared<MeshModel>(fileName);
	models.push_back(model);
}

void Scene::loadCubeModel() {
	auto model = make_shared<CubeMeshModel>();
	models.push_back(model);
}

void Scene::loadPyramidModel() {
	auto model = make_shared<PyramidMeshModel>();
	models.push_back(model);
}

int Scene::transformActiveModel(const mat4& m, bool is_rotation){
	if (models.empty()) return -1;
	models[activeModel]->transform(m, is_rotation);
	return 0;
}

void Scene::draw() {
	//TODO
	// 1. Send the renderer the current camera transform and the projection
	// 2. Tell all models to draw themselves
	m_renderer->ClearColorBuffer();

	auto active_camera = cameras[activeCamera];
	m_renderer->SetCameraTransform(active_camera->getTransform());
	m_renderer->SetProjection(active_camera->getProjection());

	for each (auto model in models) {
		model->draw(*m_renderer);
	}
	m_renderer->SwapBuffers();
}

void Scene::drawDemo() {
	m_renderer->SetDemoBuffer();
	m_renderer->SwapBuffers();
}
