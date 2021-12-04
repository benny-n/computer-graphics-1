#include "stdafx.h"
#include "Scene.h"
using namespace std;

// Scene
Scene::Scene() : Scene(&Renderer()){}

Scene::Scene(Renderer* renderer) : mRenderer(renderer), mRenderCameras(false), mControlledElement(SceneElement::Camera), mControlWorld(false),
								   mActiveModel(0), mActiveLight(0), mActiveCamera(0) {
	auto defaultCamera = make_shared<Camera>();
	mCameras.push_back(defaultCamera);
	auto ambientLight = make_shared<AmbientLight>();
	mLights.push_back(ambientLight);
}

const vector<ModelPtr>& Scene::getModels() { return mModels; }

const vector<CameraPtr>& Scene::getCameras() { return mCameras; }

const SceneElement& Scene::getControlledElement() { return mControlledElement; }

const vector<LightPtr>& Scene::getLights() { return mLights; }

void Scene::loadOBJModel(string fileName) {
	auto model = make_shared<MeshModel>(fileName);
	mModels.push_back(model);
	mActiveModel = mModels.size() - 1;
	mControlledElement = SceneElement::Model;
	printControlMsg();
}

void Scene::addCubeModel() {
	auto model = make_shared<CubeMeshModel>();
	mModels.push_back(model);
	mActiveModel = mModels.size() - 1;
	mControlledElement = SceneElement::Model;
	printControlMsg();
}

void Scene::addPyramidModel() {
	auto model = make_shared<PyramidMeshModel>();
	mModels.push_back(model);
	mActiveModel = mModels.size() - 1;
	mControlledElement = SceneElement::Model;
	printControlMsg();
}

void Scene::addCamera(){
	auto camera = make_shared<Camera>();
	mCameras.push_back(camera);
	mActiveCamera = mCameras.size() - 1;
	mControlledElement = SceneElement::Camera;
	printControlMsg();
}

void Scene::addPointLight(const vec3& p) {
	auto Light = make_shared<PointLight>(p);
	mLights.push_back(Light);
	mActiveLight = mLights.size() - 1;
	mControlledElement = SceneElement::Light;
	printControlMsg();
}

void Scene::addParallelLight(const vec3& n) {
	auto Light = make_shared<ParallelLight>(n);
	mLights.push_back(Light);
	mActiveLight = mLights.size() - 1;
	mControlledElement = SceneElement::Light;
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

void Scene::changeActiveModelMaterial() {
	mModels[mActiveModel]->setMaterialProperties();
}

void Scene::changeActiveModelMaterial(const vec4& materialProperties) {
	mModels[mActiveModel]->setMaterialProperties(materialProperties);
}

void Scene::changeActiveModelColor(const vec3& color) { mModels[mActiveModel]->setColor(color); }

void Scene::changeActiveLightColor(const vec3& color) { mLights[mActiveLight]->setColor(color); }

//SCALING
void Scene::transformActive(const mat4& m){ 
	switch (mControlledElement) {
	case SceneElement::Camera:
		mCameras[mActiveCamera]->transform(m);
		break;
	case SceneElement::Light:
		break;
	case SceneElement::Model:
		mat4 g = scale(1 / m[0][0], 1 / m[1][1], 1 / m[2][2]);
		mModels[mActiveModel]->transform(m, g, mControlWorld);
		break;
	}
}

//ROTATION
void Scene::transformActive(const float degrees, const RotationAxis& axis) { 
	switch (mControlledElement) {
	case SceneElement::Camera:
		mCameras[mActiveCamera]->transform(degrees, axis);
		break;
	case SceneElement::Light:
		break;
	case SceneElement::Model:
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
		break;
	}
}

//TRANSLATE
void Scene::transformActive(const vec3& v) {
	switch (mControlledElement) {
	case SceneElement::Camera:
		mCameras[mActiveCamera]->transform(v);
		break;
	case SceneElement::Light:
		break;
	case SceneElement::Model:
		mModels[mActiveModel]->transform(translate(v), mat4(), mControlWorld);
		break;
	}
}

inline static void getNewUp(vec4& newUp, const vec3& eye, const vec3& at) {
	if (at == vec3()) { // use eye
		if (eye.x == 0) newUp = vec3(1, 0, 0);
		else if (eye.y == 0) newUp = vec3(0, 1, 0);
		else if (eye.z == 0) newUp = vec3(0, 0, 1);
		else newUp = vec3(0, 1, -eye.y / eye.z);
	}
	else {
		if (at.x == 0) newUp = vec3(1, 0, 0);
		else if (at.y == 0) newUp = vec3(0, 1, 0);
		else if (at.z == 0) newUp = vec3(0, 0, 1);
		else newUp = vec3(0, 1, -at.y / at.z);
	}
}

void Scene::modifyActiveCamera(const vec4& v, bool isEye) {
	const CameraPtr activeCamera = mCameras[mActiveCamera];
 	vec3 vector0, modifiedEye, modifiedAt;
	if (isEye) {
		const vec4 at = activeCamera->getAt();
		modifiedAt = vec3(at.x, at.y, at.z);
		modifiedEye = vec3(v.x, v.y, v.z);
		
		vec4 newUp = cross(modifiedEye, modifiedAt);
		if (newUp == vector0)
			getNewUp(newUp, modifiedEye, modifiedAt);
		newUp = normalize(newUp);
		activeCamera->lookAt(v, at, newUp);
	}
	else { // is at
		const vec4 eye = activeCamera->getEye();
		modifiedAt = vec3(v.x, v.y, v.z);
		modifiedEye = vec3(eye.x, eye.y, eye.z);
		vec4 newUp = cross(modifiedEye, modifiedAt);
		if (newUp == vector0)
			getNewUp(newUp, modifiedEye, modifiedAt);
		newUp = normalize(newUp);
		activeCamera->lookAt(eye, v, newUp);
	}
}

void Scene::modifyActiveLight(const vec3& v, bool isPosition) {
	if (isPosition) {
		if (mLights[mActiveLight]->getType() != LightType::Point) return; // should never happen
		shared_ptr<PointLight> pointLight = dynamic_pointer_cast<PointLight>(mLights[mActiveLight]);
		pointLight->setPosition(v);
	}
	else {
		if (mLights[mActiveLight]->getType() != LightType::Parallel) return; // should never happen
		shared_ptr<ParallelLight> parallelLight = dynamic_pointer_cast<ParallelLight>(mLights[mActiveLight]);
		parallelLight->setNormal(v);
	}
}

void Scene::iterateControlledElement()
{
	switch (mControlledElement) {
	case SceneElement::Camera:
		mControlledElement = SceneElement::Light;
		break;
	case SceneElement::Light:
		if (mModels.size() > 0) mControlledElement = SceneElement::Model;
		else mControlledElement = SceneElement::Camera;
		break;
	case SceneElement::Model:
		mControlledElement = SceneElement::Camera;
		break;
	}
	printControlMsg();
}

void Scene::iterateActive() {
	switch (mControlledElement) {
	case SceneElement::Camera:
		mActiveCamera = (mActiveCamera + 1) % mCameras.size();
		break;
	case SceneElement::Light:
		mActiveLight = (mActiveLight + 1) % mLights.size();
		break;
	case SceneElement::Model:
		if (!mModels.empty()) mActiveModel = (mActiveModel + 1) % mModels.size();
		break;
	}
	printControlMsg();
}

void Scene::printControlMsg() {
	cout << "Controlling ";
	switch (mControlledElement) {
	case SceneElement::Camera:
		cout << "camera " << mActiveCamera;
		break;
	case SceneElement::Light:
		cout << mLights[mActiveLight]->getTypeString() << " light";
		if (mLights[mActiveLight]->getType() != LightType::Ambient)
			cout << " " << mActiveLight;
		break;
	case SceneElement::Model:
		cout << mModels[mActiveModel]->getName() << " " << mActiveModel;
		if (mControlWorld) cout << " (world)";
		break;
	}
	cout << endl;
}

void Scene::removeActiveModel()
{
	vector<ModelPtr>::iterator iter = mModels.begin() + mActiveModel;
	mModels.erase(iter);
	if (mActiveModel > 0) mActiveModel--;
	else if (mModels.size() > 0) mActiveModel = mModels.size() - 1;
	else mControlledElement = SceneElement::Camera;
	if (mControlledElement == SceneElement::Model) printControlMsg();
}

void Scene::removeActiveCamera()
{
	if (mCameras.size() == 1) return; // should never happen
	vector<CameraPtr>::iterator iter = mCameras.begin() + mActiveCamera;
	mCameras.erase(iter);
	if (mActiveCamera > 0) mActiveCamera--;
	else mActiveCamera = mCameras.size() - 1;
	if (mControlledElement == SceneElement::Camera) printControlMsg();
}

void Scene::removeActiveLight()
{
	if (mActiveLight == 0) return; // should never happen
	vector<LightPtr>::iterator iter = mLights.begin() + mActiveLight;
	mLights.erase(iter);
	if (mActiveLight > 0) mActiveLight--;
	else mActiveLight = mLights.size() - 1;
	if (mControlledElement == SceneElement::Light) printControlMsg();
}

void Scene::draw() {
	mRenderer->reset();

	auto activeCamera = mCameras[mActiveCamera];
	mRenderer->setCameraTransform(activeCamera->getTransform());
	mRenderer->setProjection(activeCamera->getProjection());

	for each (auto model in mModels) {
		model->draw(mRenderer);
	}
	mRenderer->drawTriangles();
	if (mRenderCameras) {
		for each (auto camera in mCameras) {
			camera->draw(mRenderer);
		}
	}
	if (!mModels.empty()) mRenderer->scanLineZBuffer();

	mRenderer->swapBuffers();
}

void Scene::drawDemo() {
	mRenderer->setDemoBuffer();
	mRenderer->swapBuffers();
}

