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

const vector<CameraPtr>& Scene::getCameras()
{
	return mCameras;
}

void Scene::loadOBJModel(string fileName) {
	auto model = make_shared<MeshModel>(fileName);
	mModels.push_back(model);
	mActiveModel = mModels.size() - 1;
	mControlCamera = false;
	printControlMsg();
}

void Scene::addCubeModel() {
	auto model = make_shared<CubeMeshModel>();
	mModels.push_back(model);
	mActiveModel = mModels.size() - 1;
	mControlCamera = false;
	printControlMsg();
}

void Scene::addPyramidModel() {
	auto model = make_shared<PyramidMeshModel>();
	mModels.push_back(model);
	mActiveModel = mModels.size() - 1;
	mControlCamera = false;
	printControlMsg();
}

void Scene::addCamera(){
	auto camera = make_shared<Camera>();
	mCameras.push_back(camera);
	mActiveCamera = mCameras.size() - 1;
	mControlCamera = true;
	printControlMsg();
}

void Scene::toggleControlWorld() {
	mControlWorld = !mControlWorld;
	printControlMsg();
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
	printControlMsg();
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

//SCALING
void Scene::transformActive(const mat4& m){ 
	if (mControlCamera) mCameras[mActiveCamera]->transform(m);
	else {
		mat4 g = scale(1 / m[0][0], 1 / m[1][1], 1 / m[2][2]);
		mModels[mActiveModel]->transform(m, g, mControlWorld);
	}
}

//ROTATION
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
		mModels[mActiveModel]->transform(rotation, rotation, mControlWorld);
	}
}

//TRANSLATE
void Scene::transformActive(const vec3& v) {
	if (mControlCamera) mCameras[mActiveCamera]->transform(v);
	else mModels[mActiveModel]->transform(translate(v), mat4(), mControlWorld);
}

void Scene::modifyActiveCamera(const vec4& v, bool isEye) {
	const CameraPtr activeCamera = mCameras[mActiveCamera];
 	vec3 vector0, modifiedEye, modifiedAt;
	if (isEye) {
		const vec4 at = activeCamera->getAt();
		modifiedAt = vec3(at.x, at.y, at.z);
		modifiedEye = vec3(v.x, v.y, v.z);
		/*while (modifiedAt == vector0 || modifiedEye == vector0) {
			modifiedAt.x++;
			modifiedEye.x++;
		}*/
		
		vec4 newUp = cross(modifiedEye, modifiedAt);
		while (newUp == vector0) {
			modifiedAt.x++;
			modifiedEye.x++;
			newUp = cross(modifiedEye, modifiedAt);
		}
		activeCamera->lookAt(v, at, newUp);
	}
	else { // is at
		const vec4 eye = activeCamera->getEye();
		modifiedAt = vec3(v.x, v.y, v.z);
		modifiedEye = vec3(eye.x, eye.y, eye.z);
		vec4 newUp = cross(modifiedEye, modifiedAt);
		while (newUp == vector0) {
			modifiedAt.x++;
			modifiedEye.x++;
			newUp = cross(modifiedEye, modifiedAt);
		}
		activeCamera->lookAt(eye, v, newUp);
	}
}

void Scene::iterateActive() {
	if (mControlCamera) { mActiveCamera = (mActiveCamera + 1) % mCameras.size(); }
	else {
		if (!mModels.empty()) mActiveModel = (mActiveModel + 1) % mModels.size();
	}
	printControlMsg();
}

void Scene::printControlMsg() {
	cout << "Controlling ";
	if (mControlCamera) cout << "camera " << mActiveCamera;
	else {
		cout << mModels[mActiveModel]->getName() << " " << mActiveModel;
		if (mControlWorld) cout << " (world)";
	}
	cout << endl;
}

void Scene::removeActiveModel()
{
	vector<ModelPtr>::iterator iter = mModels.begin() + mActiveModel;
	mModels.erase(iter);
	if (mActiveModel > 0) mActiveModel--;
	else if (mModels.size() > 0) mActiveModel = mModels.size() - 1;
	else mControlCamera = true;
	if (!mControlCamera) printControlMsg();
}

void Scene::removeActiveCamera()
{
	vector<CameraPtr>::iterator iter = mCameras.begin() + mActiveCamera;
	mCameras.erase(iter);
	if (mActiveCamera > 0) mActiveCamera--;
	else mActiveCamera = mCameras.size() - 1;
	if (mControlCamera) printControlMsg();
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

