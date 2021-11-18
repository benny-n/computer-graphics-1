#pragma once
#include "gl/glew.h"
#include <vector>
#include <string>
#include "Model.h"
#include "Camera.h"
#include "Renderer.h"
using namespace std;

class Light {

};

typedef shared_ptr<Light> LightPtr;

class Scene {
	vector<ModelPtr> mModels;
	vector<LightPtr> mLights;
	vector<CameraPtr> mCameras;
	Renderer *mRenderer;
	bool mRenderCameras;
	bool mControlCamera;
	bool mControlWorld;

public:
	Scene();
	Scene(Renderer *renderer);
	const vector<ModelPtr>& getModels();
	const vector<CameraPtr>& getCameras();
	void loadOBJModel(string fileName);
	void addCubeModel();
	void addPyramidModel();
	void addCamera();
	void toggleControlWorld();
	void toggleRenderCameras();
	void togglePlotBoundryBox();
	void togglePlotVertexNormals();
	void togglePlotFaceNormals();
	void toggleControlCamera();
	void changeColor(const vec3& color);
	void visualizeSlopes();
	void transformActive(const mat4& m); // for scaling
	void transformActive(const float degrees, const RotationAxis& axis); // for rotating
	void transformActive(const vec3& v); // for translating
	void modifyActiveCamera(const vec4& v, bool isEye);
	void iterateActive();
	void printControlMsg();
	void removeActiveModel();
	void removeActiveCamera();
	void draw();
	void drawDemo();
	
	int mActiveModel;
	int mActiveLight;
	int mActiveCamera;
};