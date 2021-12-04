#pragma once
#include "gl/glew.h"
#include <vector>
#include <string>
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Renderer.h"
using namespace std;

enum class SceneElement {
	Camera, Light, Model
};

class Scene {
	vector<ModelPtr> mModels;
	vector<LightPtr> mLights;
	vector<CameraPtr> mCameras;
	Renderer *mRenderer;
	SceneElement mControlledElement;
	bool mRenderCameras;
	bool mControlWorld;

public:
	Scene();
	Scene(Renderer *renderer);
	const vector<ModelPtr>& getModels();
	const vector<CameraPtr>& getCameras();
	const vector<LightPtr>& getLights();
	const SceneElement& getControlledElement();
	void loadOBJModel(string fileName);
	void addCubeModel();
	void addPyramidModel();
	void addCamera();
	void addPointLight(const vec3& p);
	void addParallelLight(const vec3& n);
	void toggleControlWorld();
	void toggleRenderCameras();
	void togglePlotBoundryBox();
	void togglePlotVertexNormals();
	void togglePlotFaceNormals();
	void changeActiveModelMaterial();
	void changeActiveModelMaterial(const vec4& materialProperties);
	void changeActiveModelColor(const vec3& color);
	void changeActiveLightColor(const vec3& color);
	void transformActive(const mat4& m); // for scaling
	void transformActive(const float degrees, const RotationAxis& axis); // for rotating
	void transformActive(const vec3& v); // for translating
	void modifyActiveCamera(const vec4& v, bool isEye);
	void modifyActiveLight(const vec3& v, bool isPosition);
	void iterateControlledElement();
	void iterateActive();
	void printControlMsg();
	void removeActiveModel();
	void removeActiveCamera();
	void removeActiveLight();
	void draw();
	void drawDemo();
	
	int mActiveModel;
	int mActiveLight;
	int mActiveCamera;
};