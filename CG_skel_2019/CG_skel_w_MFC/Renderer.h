#pragma once
#include <vector>
#include "CG_skel_w_MFC.h"
#include "vec.h"
#include "mat.h"
#include "GL/glew.h"

using namespace std;
class Renderer
{
	float *mOutBuffer; // 3*width*height
	float *mZbuffer; // width*height
	int mWidth, mHeight;
	mat4 mCameraTransform;
	mat4 mProjection;
	mat4 mObjectTransform;
	mat4 mNormalTransform;
	vec3 mColors[4];


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
	void setColor(const vec3& color);
	void setVisualizeSlopes();
	void reshape(int width, int height);
	void colorPixel(int x, int y, const vec3& color = vec3(1));
	void clearPixel(int x, int y);
	void drawCamera(const vec4& eye);
	void drawLine(int x1, int y1, int x2, int y2);
	void setCameraTransform(const mat4& cTransform);
	void setProjection(const mat4& projection);
	void setObjectMatrices(const mat4& oTransform, const mat4& nTransform);
	mat4 getAspectRatio();
	void calcTriangleAndNormalCoordinates(const vector<vec3>* vertices, const vector<vec3>* normals,
		vec2 triangles[3], vec3 triangles3d[3], int i, int j);
	void calcTriangleCoordinates(const vector<vec3>* vertices, vec2 triangles[3], vec3 triangles3d[3], int i, int j);
	void calcTriangleAndFaceNormalCoordinates(vec3 triangles3d[3]);
	void drawTriangles(const vector<vec3>* vertices, const vector<vec3>* normals,
		bool drawVertexNormals, bool drawFaceNormals);
	void drawSquares(const vector<vec3>* vertices);
	void swapBuffers();
	void clearColorBuffer();
	void clearDepthBuffer();
};
