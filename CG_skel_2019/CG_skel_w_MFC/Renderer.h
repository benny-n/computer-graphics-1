#pragma once
#include <vector>
#include "CG_skel_w_MFC.h"
#include "vec.h"
#include "mat.h"
#include "GL/glew.h"

using namespace std;
class Renderer
{
	float *m_outBuffer; // 3*width*height
	float *m_zbuffer; // width*height
	int m_width, m_height;
	mat4 m_cTransform;
	mat4 m_projection;
	mat4 m_oTransform;
	mat3 m_nTransform;
	vec3 colors[4];


	void CreateBuffers(int width, int height);
	void CreateLocalBuffer();

	//////////////////////////////
	// openGL stuff. Don't touch.

	GLuint gScreenTex;
	GLuint gScreenVtc;
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();
	//////////////////////////////
public:
	Renderer();
	Renderer(int width, int height);
	~Renderer(void);
	void SetDemoBuffer();
	void SetColor(const vec3& color);
	void SetVisualizeSlopes();
	void Reshape(int width, int height);
	void ColorPixel(int x, int y, const vec3& color = vec3(1));
	void ClearPixel(int x, int y);
	void DrawCamera(const vec4& eye);
	void DrawLine(int x1, int y1, int x2, int y2);
	void Init();
	void SetCameraTransform(const mat4& cTransform);
	void SetProjection(const mat4& projection);
	void SetObjectMatrices(const mat4& oTransform, const mat3& nTransform);
	mat4 CalcFinalTransformation();
	void DrawTriangles(const vector<vec3>* vertices, const vector<vec3>* normals=NULL);
	void DrawSquares(const vector<vec3>* vertices);
	void SwapBuffers();
	void ClearColorBuffer();
	void ClearDepthBuffer();
};
