#include "stdafx.h"
#include "Scene.h"
using namespace std;

// Scene
Scene::Scene() : Scene(&Renderer()){}

Scene::Scene(Renderer* renderer) : mRenderer(renderer), mRenderCameras(false), mControlCamera(true), mControlWorld(false),
								   mActiveModel(0), mActiveLight(0), mActiveCamera(0) {
	auto defaultCamera = make_shared<Camera>();
	mCameras.push_back(defaultCamera);
}

const vector<ModelPtr>& Scene::getModels()
{
	return mModels;
}

void Scene::setControlCamera(bool c)
{
	if (c == mControlCamera) return;
	toggleControlCamera();
}

void Scene::loadOBJModel(string fileName) {
	auto model = make_shared<MeshModel>(fileName);
	mModels.push_back(model);
}

void Scene::addCubeModel() {
	auto model = make_shared<CubeMeshModel>();
	mModels.push_back(model);
}

void Scene::addPyramidModel() {
	auto model = make_shared<PyramidMeshModel>();
	mModels.push_back(model);
}

void Scene::addCamera(){
	auto camera = make_shared<Camera>();
	mCameras.push_back(camera);
}

void Scene::toggleControlWorld() {
	mControlWorld = !mControlWorld;
	cout << "You are now transforming the ";
	cout << (mControlWorld ? "world" : "object");
	cout << "!" << endl;
}

void Scene::toggleRenderCameras(){
	mRenderCameras = !mRenderCameras;
}

void Scene::togglePlotBoundryBox(){
	mModels[mActiveModel]->mDrawBoundryBox = !mModels[mActiveModel]->mDrawBoundryBox;
}

void Scene::togglePlotVertexNormals(){
	mModels[mActiveModel]->mDrawVertexNormals = !mModels[mActiveModel]->mDrawVertexNormals;
}

void Scene::togglePlotFaceNormals(){
	mModels[mActiveModel]->mDrawFaceNormals = !mModels[mActiveModel]->mDrawFaceNormals;
}

void Scene::toggleControlCamera()
{
	mControlCamera = !mControlCamera;
	cout << "You are now controling the active ";
	cout << (mControlCamera ? "camera" : "model");
	cout << "!" << endl;
}

void Scene::changeColor(const vec3& color)
{
	mModels[mActiveModel]->mColor = color;
	mModels[mActiveModel]->mUseVisualizeSlopes = false;
}

void Scene::visualizeSlopes()
{
	mModels[mActiveModel]->mUseVisualizeSlopes = true;
}

void Scene::transformActive(const mat4& m){
	if (mControlCamera) mCameras[mActiveCamera]->transform(m);
	else mModels[mActiveModel]->transform(m);
}

void Scene::transformActive(const float degrees, const RotationAxis& axis) {
	if (mControlCamera) mCameras[mActiveCamera]->transform(degrees, axis);
	else {
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
		if (mControlWorld) mModels[mActiveModel]->transformWorld(rotation);
		else mModels[mActiveModel]->transform(rotation);
	}
}

void Scene::transformActive(const vec3& v) {
	if (mControlCamera) mCameras[mActiveCamera]->transform(v);
	else mModels[mActiveModel]->transform(translate(v));
}

void Scene::iterateActive(){
	if (mControlCamera) {
		mActiveCamera = (mActiveCamera + 1) % mCameras.size();
	}
	else {
		if (!mModels.empty()) mActiveModel = (mActiveModel + 1) % mModels.size();
	}
}

void Scene::removeActiveModel()
{
	vector<ModelPtr>::iterator iter = mModels.begin() + mActiveModel;
	mModels.erase(iter);
	if (mActiveModel > 0) mActiveModel--;
	else if (mModels.size() > 0) mActiveModel = mModels.size() - 1;
	else mControlCamera = true;
}

void Scene::draw() {
	//TODO
	// 1. Send the renderer the current camera transform and the projection
	// 2. Tell all models to draw themselves
	mRenderer->clearColorBuffer();

	auto activeCamera = mCameras[mActiveCamera];
	mRenderer->setCameraTransform(activeCamera->getTransform());
	mRenderer->setProjection(activeCamera->getProjection());

	for each (auto model in mModels) {
		model->draw(mRenderer);
	}
	if (mRenderCameras) {
		for each (auto camera in mCameras) {
			camera->draw(mRenderer);
		}
	}
	mRenderer->swapBuffers();
}

void Scene::drawDemo() {
	mRenderer->setDemoBuffer();
	mRenderer->swapBuffers();
}

