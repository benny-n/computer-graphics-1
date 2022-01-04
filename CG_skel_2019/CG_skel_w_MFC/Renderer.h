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
	void setDemoBuffer();
	void reshape(int width, int height);
	void colorPixel(int x, int y, const Color& color = Color{ 0.5, 0.5, 0.5 });
	void clearPixel(int x, int y);
	void drawCamera(const vec4& eye);
	void drawLine(int x1, int y1, int x2, int y2);
	void clipAndDrawLine(vec3 p1, vec3 p2);
	void setCameraTransform(const mat4& cTransform);
	void setProjection(const mat4& projection);
	void setObjectMatrices(const ModelPtr model);
	void calcTriangleAndFaceNormalCoordinates(vec3 triangles3d[3], const mat4& from3dTo2d);
	void drawTriangles(const vector<Poly>& polygons);
	void drawSquares(const vector<vec3>* vertices);
	void blur();
	void bloom();
	void swapBuffers();
	void clearColorBuffer();
	void clearDepthBuffer();
};
