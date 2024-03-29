#pragma once
#include "gl/glew.h"
#include <vector>
#include <string>
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Rasterizer.h"
using namespace std;

enum class SceneElement {
	Camera, Light, Model
};

class Scene {
	vector<ModelPtr> mModels;
	vector<LightPtr> mLights;
	vector<CameraPtr> mCameras;
	RasterizerPtr mRasterizer;
	SceneElement mControlledElement;
	mat4 mAspectRatioTransform;
	GLuint mScreenTex;
	GLuint mScreenVtc;
	GLuint mSkyboxTex;
	bool mUseSkybox;
	bool mRenderCameras;
	bool mControlWorld;
	vector<GLfloat> mSkyboxBuffer;

public:
	int mWidth, mHeight;
	Scene();
	const vector<ModelPtr>& getModels();
	const vector<CameraPtr>& getCameras();
	const vector<LightPtr>& getLights();
	const SceneElement& getControlledElement();
	const Material& getActiveModelMaterial();
	const int getActiveModelNumVertices();
	void setRasterizer(ShaderType shaderType);
	void loadOBJModel(string fileName);
	void loadTexture(string fileName);
	void loadNormalMap(string fileName);
	void loadSkybox(string title);
	void removeSkybox();
	bool skyboxActive();
	void initOpenGLRendering();
	void createBuffers(int width, int height);
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
	void changeActiveModelMaterial(const Color& color);
	void changeActiveModelMaterial(const Material& material);
	void changeActiveModelVertexMaterial(int index, const Material& nextMaterial);
	void changeActiveLightColor(const Color& color);
	void changeActiveLightColor(const Color& c1, const Color& c2, const Color& c3);
	void growActiveModelVertex(int vertexIndex);
	void transformActive(const mat4& m); // for scaling
	void transformActive(const float degrees, const RotationAxis& axis); // for rotating
	void transformActive(const vec3& v); // for translating
	void modifyActiveCamera(const vec4& v, bool isEye);
	void modifyActiveLight(const vec3& v, bool isPosition);
	void reshape(int width, int height);
	void iterateControlledElement();
	void iterateActive();
	void printControlMsg();
	void removeActiveModel();
	void removeActiveCamera();
	void removeActiveLight();
	void setGlLights(GLuint program);
	void drawSkybox();
	void draw();
	void drawDemo();

	int mActiveModel;
	int mActiveLight;
	int mActiveCamera;
};