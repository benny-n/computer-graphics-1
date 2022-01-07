#pragma once
#include "gl/glew.h"
#include <vector>
#include <string>
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Rasterizer.h"
#include "Renderer.h"
using namespace std;

enum class SceneElement {
	Camera, Light, Model
};

class Scene {
	vector<ModelPtr> mModels;
	vector<LightPtr> mLights;
	vector<CameraPtr> mCameras;
	vector<Poly> mPolygons;
	RasterizerPtr mRasterizer;
	Renderer* mRenderer;
	SceneElement mControlledElement;
	bool mRenderCameras;
	bool mControlWorld;
	BlurIntensity mBlurIntensity;
	bool mBloom;

public:
	Scene();
	Scene(Renderer *renderer);
	const vector<ModelPtr>& getModels();
	const vector<CameraPtr>& getCameras();
	const vector<LightPtr>& getLights();
	const vector<Poly>& getPolygons();
	const SceneElement& getControlledElement();
	void setRasterizer(ShaderType shaderType);
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
	void setBlur(const BlurIntensity& intensity);
	void toggleBloom();
	void changeActiveModelMaterial();
	void changeActiveModelMaterial(const Color& color);
	void changeActiveModelMaterial(const Material& material);
	void changeActiveLightColor(const Color& color);
	void changeActiveLightColor(const Color& c1, const Color& c2, const Color& c3);
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
	void setGlLights();
	void draw();
	void drawDemo();

	int mActiveModel;
	int mActiveLight;
	int mActiveCamera;
};