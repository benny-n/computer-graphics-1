#include "StdAfx.h"
#include "Renderer.h"
#include "CG_skel_w_MFC.h"
#include "InitShader.h"
#include "GL\freeglut.h"

#define INDEX(width,x,y,c) (x+y*width)*3+c

Renderer::Renderer() : mOutBuffer(nullptr), mWidth(512), mHeight(512) {
	initOpenGLRendering();
	createBuffers(512,512);
	setVisualizeSlopes();
}

Renderer::Renderer(int width, int height) : mOutBuffer(nullptr), mWidth(width), mHeight(height) {
	initOpenGLRendering();
	createBuffers(width,height);
	setVisualizeSlopes();
}

Renderer::~Renderer(void)
{
	free(mOutBuffer);
}

void Renderer::createBuffers(int width, int height)
{
	mWidth=width;
	mHeight=height;
	createOpenGLBuffer(); //Do not remove this line.
	if (mOutBuffer == nullptr) {
		RECT desktop;
		const HWND hdesktop = GetDesktopWindow();
		GetWindowRect(hdesktop, &desktop);
		mOutBuffer = (GLfloat*)malloc(sizeof(GLfloat) * (3 * desktop.right * desktop.bottom));
		swapBuffers();
	}
	glutReshapeWindow(width, height);
}

void Renderer::setDemoBuffer()
{
	//vertical line
	for(int i=0; i<mWidth; i++)
	{
		mOutBuffer[INDEX(mWidth,256,i,0)]=1;	mOutBuffer[INDEX(mWidth,256,i,1)]=0;	mOutBuffer[INDEX(mWidth,256,i,2)]=0;

	}
	//horizontal line
	for(int i=0; i<mWidth; i++)
	{
		mOutBuffer[INDEX(mWidth,i,256,0)]=1;	mOutBuffer[INDEX(mWidth,i,256,1)]=0;	mOutBuffer[INDEX(mWidth,i,256,2)]=1;

	}

}

void Renderer::setColor(const vec3& color)
{
	mColors[0] = mColors[1] = mColors[2] = mColors[3] = color;
}

void Renderer::setVisualizeSlopes()
{
	mColors[0] = vec3(1);
	mColors[1] = vec3(1, 0, 0);
	mColors[2] = vec3(0, 1, 0);
	mColors[3] = vec3(0, 0, 1);
}

void Renderer::reshape(int width, int height){ 
	createBuffers(width, height); 
	if (mWidth > mHeight) mAspectRatioTransform = scale(1, mWidth / (float)mHeight, 1);
	else  mAspectRatioTransform = scale(mHeight / (float)mWidth, 1, 1);
}

void Renderer::colorPixel(int x, int y, const vec3& color) {
	if (x >= mWidth || x < 0) return; //clip
	if (y >= mHeight || y < 0) return; //clip
	mOutBuffer[INDEX(mWidth, x, y, 0)] = color.x;
	mOutBuffer[INDEX(mWidth, x, y, 1)] = color.y;
	mOutBuffer[INDEX(mWidth, x, y, 2)] = color.z;
}

void Renderer::clearPixel(int x, int y) {
	colorPixel(x, y, vec3());
}

void Renderer::drawCamera(const vec4& eye){
	const mat4 finalTransformation = mProjection * mCameraTransform;
	vec4 transformedEye = (finalTransformation * eye) / eye.w;
	drawLine(transformedEye.x - 5, transformedEye.y, transformedEye.x + 5, transformedEye.y);
	drawLine(transformedEye.x, transformedEye.y - 5, transformedEye.x, transformedEye.y + 5);
}

static const float epsilon = 0.0000000001;
static inline bool isZeroish(float x) { return (x <= epsilon && x >= -epsilon); }

static inline bool outsideClipVolume(vec3& p1, vec3& p2) {
	vec4 pDelta = p2 - p1;
	float a[6];
	a[0] = -pDelta.y;
	a[1] = -pDelta.x;
	a[2] = pDelta.y;
	a[3] = pDelta.x;
	a[4] = -pDelta.z;
	a[5] = pDelta.z;

	float b[6];
	b[0] = p1.y + 1;
	b[1] = p1.x + 1;
	b[2] = 1 - p1.y;
	b[3] = 1 - p1.x;
	b[4] = p1.z + 1;
	b[5] = 1 - p1.z;

	for (int i = 0; i < 6; i++)
		if (isZeroish(a[i]) && b[i] < 0) return true;

	return false;
}

static inline bool clipTop(vec3& p1, vec3& p2) {
	if (p1.y > 1 && p2.y > 1) return false;
	if (p1.y <= 1 && p2.y <= 1) return true;
	vec3 normal(0, 1, 0);
	vec3 p0(0, 1, 0);

	float alpha = (dot(normal, p0 - p1) / dot(normal, p2 - p1));

	vec3 pAlpha = (1 - alpha) * p1 + alpha * p2;

	p1.y > p2.y ? p1 = pAlpha : p2 = pAlpha;

	return true;
}

static inline bool clipBottom(vec3& p1, vec3& p2) {
	if (p1.y < -1 && p2.y < -1) return false;
	if (p1.y >= -1 && p2.y >= -1) return true;
	vec3 normal(0, -1, 0);
	vec3 p0(0, -1, 0);

	float alpha = (dot(normal, p0 - p1) / dot(normal, p2 - p1));

	vec3 pAlpha = (1 - alpha) * p1 + alpha * p2;

	p1.y < p2.y ? p1 = pAlpha : p2 = pAlpha;

	return true;
}

static inline bool clipRight(vec3& p1, vec3& p2) {
	if (p1.x > 1 && p2.x > 1) return false;
	if (p1.x <= 1 && p2.x <= 1) return true;
	vec3 normal(1, 0, 0);
	vec3 p0(1, 0, 0);

	float alpha = (dot(normal, p0 - p1) / dot(normal, p2 - p1));

	vec3 pAlpha = (1 - alpha) * p1 + alpha * p2;

	p1.x > p2.x ? p1 = pAlpha : p2 = pAlpha;

	return true;
}

static inline bool clipLeft(vec3& p1, vec3& p2) {
	if (p1.x < -1 && p2.x < -1) return false;
	if (p1.x >= -1 && p2.x >= -1) return true;
	vec3 normal(-1, 0, 0);
	vec3 p0(-1, 0, 0);

	float alpha = (dot(normal, p0 - p1) / dot(normal, p2 - p1));

	vec3 pAlpha = (1 - alpha) * p1 + alpha * p2;

	p1.x < p2.x ? p1 = pAlpha : p2 = pAlpha;

	return true;
}

static inline bool clipFar(vec3& p1, vec3& p2) {
	if (p1.z > 1 && p2.z > 1) return false;
	if (p1.z <= 1 && p2.z <= 1) return true;
	vec3 normal(0, 0, 1);
	vec3 p0(0, 0, 1);

	float alpha = (dot(normal, p0 - p1) / dot(normal, p2 - p1));

	vec3 pAlpha = (1 - alpha) * p1 + alpha * p2;

	p1.z > p2.z ? p1 = pAlpha : p2 = pAlpha;

	return true;
}

static inline bool clipNear(vec3& p1, vec3& p2) {
	if (p1.z < -1 && p2.z < -1) return false;
	if (p1.z >= -1 && p2.z >= -1) return true;
	vec3 normal(0, 0, -1);
	vec3 p0(0, 0, -1);

	float alpha = (dot(normal, p0 - p1) / dot(normal, p2 - p1));

	vec3 pAlpha = (1 - alpha) * p1 + alpha * p2;

	p1.z < p2.z ? p1 = pAlpha : p2 = pAlpha;

	return true;
}

static inline bool clippingPipeline(vec3& p1, vec3& p2) {
	return
		clipTop(p1, p2) &&
		clipBottom(p1, p2) &&
		clipRight(p1, p2) &&
		clipLeft(p1, p2) &&
		clipFar(p1, p2) &&
		clipNear(p1, p2);
}

static inline void choosePixlesForCanonicalLine(vector<int>& ys, int x1, int y1) { // Bresenham Algorithm
	int d = 2 * y1 - x1;
	int de = 2 * y1;
	int dne = de - 2 * x1;

	for (int x = 0, y = 0; x < x1; x++)
	{
		if (d < 0) d += de;
		else {
			y++;
			d += dne;
		}
		ys[x] = y;
	}
}

void Renderer::drawLine(int x1, int y1, int x2, int y2, bool isNonModelLine) {
	if (x1 > x2) {
		swap(x1, x2);
		swap(y1, y2);
	}
	float m;
	if (x1 == x2) {
		m = FLT_MAX; // anything greater than 1 so we reflect by y=x
		if (y1 > y2) swap(y1, y2);
	}
	else m = (y2 - y1) / float(x2 - x1);

	if (m >= 0) {
		if (m <= 1) { // Just need to move to origin
			int numPixels = abs(x2 - x1);
			auto ys = vector<int>(numPixels);
			choosePixlesForCanonicalLine(ys, x2 - x1, y2 - y1);
			for (int x = 0; x < numPixels; x++)
			{
				colorPixel(x + x1, ys[x] + y1, (isNonModelLine ? vec3(0.5) : mColors[0]));
			}
		}
		else { // move to origin and reflect by y=x
			int numPixels = abs(y2 - y1);
			auto ys = vector<int>(numPixels);
			choosePixlesForCanonicalLine(ys, y2 - y1, x2 - x1); // first translate, then reflect (swap x and y)
			for (int x = 0; x < numPixels; x++)
			{
				colorPixel(ys[x] + x1, x + y1, (isNonModelLine ? vec3(0.5) : mColors[1])); // first reflect back, then translate back
			}
		}
	}

	else {
		if (m >= -1) { // move to origin and reflect by x=0
			int numPixels = abs(x2 - x1);
			auto ys = vector<int>(numPixels);
			choosePixlesForCanonicalLine(ys, x2 - x1, y1 - y2); // first translate, then reflect (minus on y)
			for (int x = 0; x < numPixels; x++)
			{
				colorPixel(x + x1, -(ys[x]) + y1, (isNonModelLine ? vec3(0.5) : mColors[2])); // first reflect back, then translate back
			}
		}
		else { // move to origin, reflect by x=0 and then reflect by y=x
			int numPixels = abs(y2 - y1);
			auto ys = vector<int>(numPixels);
			choosePixlesForCanonicalLine(ys, y1 - y2, x2 - x1); // first translate, then reflect (minus on y) and reflect again (swap x and y)
			for (int x = 0; x < numPixels; x++)
			{
				colorPixel(ys[x] + x1, -x + y1, (isNonModelLine ? vec3(0.5) : mColors[3])); // first reflect back on y=x, then reflect back on x=0, then translate back
			}
		}
	}
}

void Renderer::clipAndDrawLine(vec3 p1, vec3 p2, bool isNonModelLine) {
	if (outsideClipVolume(p1, p2)) return;
	if (!clippingPipeline(p1, p2)) return;
	int x1 = (mWidth / 2.f) * (p1.x + 1);
	int y1 = (mHeight / 2.f) * (p1.y + 1);
	int x2 = (mWidth / 2.f) * (p2.x + 1);
	int y2 = (mHeight / 2.f) * (p2.y + 1);

	drawLine(x1, y1, x2, y2, isNonModelLine);
}

void Renderer::setCameraTransform(const mat4& cTransform) { mCameraTransform = cTransform; }
void Renderer::setProjection(const mat4& projection) { mProjection = projection;  }
void Renderer::setObjectMatrices(const mat4& oTransform, const mat4& nTransform, const mat4& wTransform) {
	mObjectTransform = oTransform;
	mNormalTransform = nTransform;
	mWorldTransform = wTransform;
}

void Renderer::calcTriangleAndFaceNormalCoordinates(vec3 triangles3d[3], const mat4& from3dTo2d) {
	vec3 i = triangles3d[1] - triangles3d[0];
	vec3 j = triangles3d[2] - triangles3d[0];
	vec4 normal = vec4(normalize(cross(i, j)), 0.f);
	vec4 center = (triangles3d[0] + triangles3d[1] + triangles3d[2]) / 3;
	normal += center;
	center = from3dTo2d * center;
	normal = from3dTo2d * normal;
	center /= center.w;
	normal /= normal.w;
	clipAndDrawLine(vec3(center.x, center.y, center.z), vec3(normal.x, normal.y, normal.z), true);
}

void Renderer::drawTriangles(
	const vector<vec3>* vertices, 
	const vector<vec3>* normals, 
	bool drawVertexNormals,
	bool drawFaceNormals
) {
	vec3 triangles[3];
	vec3 triangles3d[3];
	const mat4 worldTransform = mAspectRatioTransform * mWorldTransform;
	const mat4 normalTransform = mAspectRatioTransform * mWorldTransform * mNormalTransform;
	const mat4 from3dTo2d = mProjection * mCameraTransform;
	const mat4 worldAndProjection = from3dTo2d * worldTransform;

	for (int i = 0; i < vertices->size(); i+=3) {
		for (int j = 0; j < 3; j++) {
			if (drawVertexNormals) {
				vec4 vertex((*vertices)[i + j]);
				vec4 normal(vec3((*normals)[i + j]), 0);
				vertex = mObjectTransform * vertex;
				triangles3d[j] = vec3(vertex.x, vertex.y, vertex.z);
				vertex = worldTransform * vertex;
				normal = normalTransform * normal;
				normal = normalize(normal);
				normal += vertex;
				vertex = from3dTo2d * vertex;
				normal = from3dTo2d * normal;
				vertex /= vertex.w;
				normal /= normal.w;
				vec3 normalizedVertex(vertex.x, vertex.y, vertex.z);
				clipAndDrawLine(normalizedVertex, vec3(normal.x, normal.y, normal.z), true);
				triangles[j] = normalizedVertex;
			}
			else {
				vec4 vertex((*vertices)[i + j]);
				vertex = mObjectTransform * vertex;
				triangles3d[j] = vec3(vertex.x, vertex.y, vertex.z);
				vertex = worldAndProjection * vertex;
				vertex /= vertex.w;
				triangles[j] = vec3(vertex.x, vertex.y, vertex.z);
			}
		}
		clipAndDrawLine(triangles[0], triangles[1]);
		clipAndDrawLine(triangles[1], triangles[2]);
		clipAndDrawLine(triangles[2], triangles[0]);

		if (drawFaceNormals)
			calcTriangleAndFaceNormalCoordinates(triangles3d, worldAndProjection);
	}
}

void Renderer::drawSquares(const vector<vec3>* vertices) {
	const mat4 finalTransformation = mProjection * mCameraTransform * mAspectRatioTransform * mWorldTransform * mObjectTransform;
	vec3 squares[4];
	float r, s;

	for (int i = 0; i < vertices->size(); i += 4)
	{
		for (int j = 0; j < 4; j++)
		{
			vec4 vertex((*vertices)[i + j]);
			vertex = finalTransformation * vertex;
			vertex /= vertex.w;
			squares[j] = vec3(vertex.x, vertex.y, vertex.z);
		}
		clipAndDrawLine(squares[0], squares[1], true);
		clipAndDrawLine(squares[1], squares[2], true);
		clipAndDrawLine(squares[2], squares[3], true);
		clipAndDrawLine(squares[3], squares[0], true);
	}
}


void Renderer::clearColorBuffer() {
	for (int x = 0; x < mWidth; x++) {
		for (int y = 0; y < mHeight; y++) {
			clearPixel(x, y);
		}
	}
}



/////////////////////////////////////////////////////
//OpenGL stuff. Don't touch.

void Renderer::initOpenGLRendering()
{
	int a = glGetError();
	a = glGetError();
	glGenTextures(1, &gScreenTex);
	a = glGetError();
	glGenVertexArrays(1, &gScreenVtc);
	GLuint buffer;
	glBindVertexArray(gScreenVtc);
	glGenBuffers(1, &buffer);
	const GLfloat vtc[]={
		-1, -1,
		1, -1,
		-1, 1,
		-1, 1,
		1, -1,
		1, 1
	};
	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
	glUseProgram( program );
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );

	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0,
		0 );

	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		(GLvoid *) sizeof(vtc) );
	glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );
	a = glGetError();
}

void Renderer::createOpenGLBuffer()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, mWidth, mHeight);
}

void Renderer::swapBuffers()
{

	int a = glGetError();
	glActiveTexture(GL_TEXTURE0);
	a = glGetError();
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	a = glGetError();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, GL_RGB, GL_FLOAT, mOutBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	a = glGetError();

	glBindVertexArray(gScreenVtc);
	a = glGetError();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	a = glGetError();
	glutSwapBuffers();
	a = glGetError();
}