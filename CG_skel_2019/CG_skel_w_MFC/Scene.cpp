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

void Model::BoundryBox::draw(Renderer* renderer){
	renderer->SetColor(vec3(0.5));
	renderer->DrawSquares(&vertex_positions);
}

// Model
const string& Model::getName(){
	return name;
}


// Camera
Camera::Camera() {
	LookAt(vec4(vec3(0, 0, 10)), vec4(vec3(0, 0, 0)), vec4(vec3(0, 1, 0)));
	Ortho(-10, 10, -10, 10, -8, -12);
}

Camera::Camera(const vec4& eye) {
	LookAt(eye, vec4(vec3()), vec4(vec3(0, 1, 0)));
	Ortho(-1, 1, -1, 1, -8, -12);
}

void Camera::setTransformation(const mat4& transform) {
	cTransform = transform;
}

mat4 Camera::getTransform() { return cTransform * lookAt; }
mat4 Camera::getProjection() { return projection; }

void Camera::draw(Renderer* renderer){
	renderer->SetColor(vec3(1));
	renderer->DrawCamera(eye);
}

void Camera::LookAt(const vec4& eye, const vec4& at, const vec4& up) {
	this->eye = eye;
	const vec4 n = normalize(eye - at);
	const vec4 u = vec4(normalize(cross(up, n)),0);
	const vec4 v = vec4(normalize(cross(n, u)),0);
	const vec4 t = vec4(0.0, 0.0, 0.0, 1.0);
	const mat4 c = mat4(u, v, n, t);
	lookAt = c * Translate(-eye);
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
Scene::Scene() : Scene(&Renderer()){}

Scene::Scene(Renderer* renderer) : m_renderer(renderer), render_cameras(false), control_camera(true), activeModel(0), activeLight(0), activeCamera(0) {
	auto default_camera = make_shared<Camera>();
	cameras.push_back(default_camera);

	//auto camera_2 = make_shared<Camera>(vec3(20, 20, 20));
	//cameras.push_back(camera_2);

	//auto camera_3 = make_shared<Camera>(vec3(0, 0, -20));
	//cameras.push_back(camera_3);
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

void Scene::addCamera(){
	auto camera = make_shared<Camera>();
	cameras.push_back(camera);
}

void Scene::toggleRenderCameras(){
	render_cameras = !render_cameras;
}

void Scene::togglePlotBoundryBox(){
	models[activeModel]->draw_boundry_box = !models[activeModel]->draw_boundry_box;
}

void Scene::togglePlotVertexNormals(){
	models[activeModel]->draw_vertex_normals = !models[activeModel]->draw_vertex_normals;
}

void Scene::togglePlotFaceNormals(){
	models[activeModel]->draw_face_normals = !models[activeModel]->draw_face_normals;
}

void Scene::changeColor(const vec3& color)
{
	models[activeModel]->color = color;
	models[activeModel]->use_visualize_slopes = false;
}

void Scene::visualizeSlopes()
{
	models[activeModel]->use_visualize_slopes = true;
}

void Scene::transformActive(const mat4& m){
	if (control_camera) {
		// TODO cameras[activeCamera]->transform(m)
	}
	else models[activeModel]->transform(m);
}

void Scene::iterateActive(){
	if (control_camera) {
		activeCamera = (activeCamera + 1) % cameras.size();
	}
	else {
		if (!models.empty()) activeModel = (activeModel + 1) % models.size();
	}
}

void Scene::removeActiveModel()
{
	vector<ModelPtr>::iterator iter = models.begin() + activeModel;
	models.erase(iter);
	iterateActive();
	if (models.empty()) control_camera = true;
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
		model->draw(m_renderer);
	}
	if (render_cameras) {
		for each (auto camera in cameras) {
			camera->draw(m_renderer);
		}
	}
	m_renderer->SwapBuffers();
}

void Scene::drawDemo() {
	m_renderer->SetDemoBuffer();
	m_renderer->SwapBuffers();
}
