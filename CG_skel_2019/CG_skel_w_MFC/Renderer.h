#pragma once
#include <vector>
#include "CG_skel_w_MFC.h"
#include "vec.h"
#include "mat.h"
#include "Triangle.h"
#include "Material.h"
#include "GL/glew.h"

using namespace std;
class Renderer
{
	float *mOutBuffer; // 3*width*height
	float *mZbuffer; // width
	int mWidth, mHeight;
	mat4 mCameraTransform;
	mat4 mProjection;
	mat4 mObjectTransform;
	mat4 mNormalTransform;
	mat4 mWorldTransform;
	mat4 mAspectRatioTransform;
	vector<Triangle> mPolygons;


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
	Renderer();
	Renderer(int width, int height);
	~Renderer(void);
	void setDemoBuffer();
	void reshape(int width, int height);
	void colorPixel(int x, int y, const Color& color = Color{ 0.5, 0.5, 0.5 });
	void clearPixel(int x, int y);
	void drawCamera(const vec4& eye);
	void drawLine(int x1, int y1, int x2, int y2);
	void clipAndDrawLine(vec3 p1, vec3 p2);
	void setCameraTransform(const mat4& cTransform);
	void setProjection(const mat4& projection);
	void setObjectMatrices(const mat4& oTransform, const mat4& nTransform, const mat4& wTransform);
	void calcTriangleAndFaceNormalCoordinates(vec3 triangles3d[3], const mat4& from3dTo2d);
	void preparePolygons(const vector<vec3>* vertices, const vector<vec3>* normals, const vector<Material>* materials, bool drawVertexNormals, bool drawFaceNormals);
	void scanLineZBuffer();
	void drawTriangles();
	void drawSquares(const vector<vec3>* vertices);
	void swapBuffers();
	void clearColorBuffer();
	void clearDepthBuffer();
	void reset();
};
