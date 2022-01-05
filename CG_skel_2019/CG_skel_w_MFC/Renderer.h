#pragma once
#include <vector>
#include "CG_skel_w_MFC.h"
#include "vec.h"
#include "mat.h"
#include "Polygon.h"
#include "Model.h"
#include "Material.h"
#include "GL/glew.h"

using namespace std;

enum class BlurIntensity {
	None = 0, Weak = 1, Medium = 3, Strong = 5
};

class Renderer
{
	float *mOutBuffer; // 3*width*height
	mat4 mCameraTransform;
	mat4 mProjection;
	mat4 mObjectTransform;
	mat4 mNormalTransform;
	mat4 mWorldTransform;
	mat4 mAspectRatioTransform;

	void createBuffers(int width, int height);
	void createLocalBuffer();

	//////////////////////////////
	// openGL stuff. Don't touch.

	GLuint gScreenTex;
	GLuint gScreenVtc;
	void createOpenGLBuffer();
	void initOpenGLRendering();
	//////////////////////////////
public:
	float *mZbuffer; // width
	int mWidth, mHeight;

	Renderer();
	Renderer(int width, int height);
	~Renderer(void);
	const mat4& getWorldTransform();
	const mat4& getModelTransform();
	const mat4& getNormalTransform();
	const mat4& getCameraTransform();
	const mat4& getProjection();
	const mat4 from3dTo2d();
	void reshape(int width, int height);
	void setCameraTransform(const mat4& cTransform);
	void setProjection(const mat4& projection);
	void setObjectMatrices(const ModelPtr model);
	void blur();
	void bloom();
	void swapBuffers();
};
