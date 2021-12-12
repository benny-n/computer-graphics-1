#include "StdAfx.h"
#include "Renderer.h"
#include "CG_skel_w_MFC.h"
#include "InitShader.h"
#include "GL\freeglut.h"

#define INDEX(width,x,y,c) (x+y*width)*3+c

Renderer::Renderer() : mOutBuffer(nullptr), mWidth(512), mHeight(512) {
	initOpenGLRendering();
	createBuffers(512,512);
}

Renderer::Renderer(int width, int height) : mOutBuffer(nullptr), mWidth(width), mHeight(height) {
	initOpenGLRendering();
	createBuffers(width,height);
}

Renderer::~Renderer(void)
{
	free(mOutBuffer);
	free(mZbuffer);
}

const mat4& Renderer::getWorldTransform() { return mAspectRatioTransform * mWorldTransform; }
const mat4& Renderer::getModelTransform() { return getWorldTransform() * mObjectTransform; }
const mat4& Renderer::getNormalTransform() { return getWorldTransform() * mNormalTransform; }
const mat4& Renderer::getCameraTransform() { return mCameraTransform; }
const mat4& Renderer::getProjection() { return mProjection; }

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
		mZbuffer = (GLfloat*)malloc(sizeof(GLfloat) * (desktop.right));
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

void Renderer::reshape(int width, int height){ 
	createBuffers(width, height); 
	if (mWidth > mHeight) mAspectRatioTransform = scale(1, mWidth / (float)mHeight, 1);
	else  mAspectRatioTransform = scale(mHeight / (float)mWidth, 1, 1);
}

void Renderer::colorPixel(int x, int y, const Color& color) {
	if (x >= mWidth || x < 0) return; //clip
	if (y >= mHeight || y < 0) return; //clip
	mOutBuffer[INDEX(mWidth, x, y, 0)] = color.r;
	mOutBuffer[INDEX(mWidth, x, y, 1)] = color.g;
	mOutBuffer[INDEX(mWidth, x, y, 2)] = color.b;
}

void Renderer::clearPixel(int x, int y) {
	colorPixel(x, y, Color{ 0,0,0 });
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

void Renderer::drawLine(int x1, int y1, int x2, int y2) {
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
				colorPixel(x + x1, ys[x] + y1);
			}
		}
		else { // move to origin and reflect by y=x
			int numPixels = abs(y2 - y1);
			auto ys = vector<int>(numPixels);
			choosePixlesForCanonicalLine(ys, y2 - y1, x2 - x1); // first translate, then reflect (swap x and y)
			for (int x = 0; x < numPixels; x++)
			{
				colorPixel(ys[x] + x1, x + y1); // first reflect back, then translate back
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
				colorPixel(x + x1, -(ys[x]) + y1); // first reflect back, then translate back
			}
		}
		else { // move to origin, reflect by x=0 and then reflect by y=x
			int numPixels = abs(y2 - y1);
			auto ys = vector<int>(numPixels);
			choosePixlesForCanonicalLine(ys, y1 - y2, x2 - x1); // first translate, then reflect (minus on y) and reflect again (swap x and y)
			for (int x = 0; x < numPixels; x++)
			{
				colorPixel(ys[x] + x1, -x + y1); // first reflect back on y=x, then reflect back on x=0, then translate back
			}
		}
	}
}

void Renderer::clipAndDrawLine(vec3 p1, vec3 p2) {
	if (outsideClipVolume(p1, p2)) return;
	if (!clippingPipeline(p1, p2)) return;
	int x1 = (mWidth / 2) * (p1.x + 1);
	int y1 = (mHeight / 2) * (p1.y + 1);
	int x2 = (mWidth / 2) * (p2.x + 1);
	int y2 = (mHeight / 2) * (p2.y + 1);

	drawLine(x1, y1, x2, y2);
}

void Renderer::setCameraTransform(const mat4& cTransform) { mCameraTransform = cTransform; }
void Renderer::setProjection(const mat4& projection) { mProjection = projection;  }
void Renderer::setObjectMatrices(const ModelPtr model) {
	mObjectTransform = model->getModelTransform();
	mNormalTransform = model->getNormalTransform();
	mWorldTransform = model->getWorldTransform();
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
	clipAndDrawLine(vec3FromVec4(center), vec3FromVec4(normal));
}

void Renderer::drawTriangles(const vector<Poly>& polygons) {
	const mat4 from3dTo2d = mProjection * mCameraTransform;

	for each (auto& poly in polygons){
		if (poly.mDrawVertexNormal) {
			for (int i = 0; i < 3; i++) {
				vec4 normal(poly.mTriangle.mVertexNormals[i]);
				normal += vec4(poly.mTriangle.mVertices[i], 0);
				normal = from3dTo2d * normal;
				normal = normal / normal.w;
				clipAndDrawLine(poly.mProjectedTriangle.mVertices[i], vec3FromVec4(normal));
			}
		}
		//clipAndDrawLine(poly.mProjectedTriangle.mVertices[0], poly.mProjectedTriangle.mVertices[1]);
		//clipAndDrawLine(poly.mProjectedTriangle.mVertices[1], poly.mProjectedTriangle.mVertices[2]);
		//clipAndDrawLine(poly.mProjectedTriangle.mVertices[2], poly.mProjectedTriangle.mVertices[0]);

		if (poly.mDrawFaceNormal) {
			vec3 center = poly.mProjectedTriangle.center();
			vec3 normal = center + 0.1 * poly.mProjectedTriangle.getFaceNormal();
			clipAndDrawLine(center, normal);
		}
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
			squares[j] = vec3FromVec4(vertex);
		}
		clipAndDrawLine(squares[0], squares[1]);
		clipAndDrawLine(squares[1], squares[2]);
		clipAndDrawLine(squares[2], squares[3]);
		clipAndDrawLine(squares[3], squares[0]);
	}
}

void Renderer::blur() {
	vector<float> gaussian = vector<float>{ 1.0 / 256, 1.0 / 32, 7.0 / 64, 7.0 / 32, 35.0 / 128, 7.0 / 32, 7.0 / 64, 1.0 / 32, 1.0 / 256 };
		
	int x_tag, y_tag;
	float* modifiedBuffer = new float[mWidth * mHeight * 3];
	for (int y = 0; y < mHeight; y++) {
		for (int x = 0; x < mWidth; x++) {
			Color pixel{ 0, 0, 0 };
			for (int i = 0; i < gaussian.size(); i++) {
				x_tag = x - ((gaussian.size() - 1) / 2.0) + i;
				if (x_tag < 0 || x_tag >= mWidth) continue;
				pixel.r += mOutBuffer[INDEX(mWidth, x_tag, y, 0)] * gaussian[i];
				pixel.g += mOutBuffer[INDEX(mWidth, x_tag, y, 1)] * gaussian[i];
				pixel.b += mOutBuffer[INDEX(mWidth, x_tag, y, 2)] * gaussian[i];

			}
			modifiedBuffer[INDEX(mWidth, x, y, 0)] = pixel.r;
			modifiedBuffer[INDEX(mWidth, x, y, 1)] = pixel.g;
			modifiedBuffer[INDEX(mWidth, x, y, 2)] = pixel.b;
		}
	}	
	for (int x = 0; x < mWidth; x++) {
		for (int y = 0; y < mHeight; y++) {
			Color pixel{ 0, 0, 0 };
			for (int i = 0; i < gaussian.size(); i++) {
				y_tag = y - ((gaussian.size() - 1) / 2.0) + i;
				if (y_tag < 0 || y_tag >= mHeight) continue;
				pixel.r += modifiedBuffer[INDEX(mWidth, x, y_tag, 0)] * gaussian[i];
				pixel.g += modifiedBuffer[INDEX(mWidth, x, y_tag, 1)] * gaussian[i];
				pixel.b += modifiedBuffer[INDEX(mWidth, x, y_tag, 2)] * gaussian[i];

			}
			mOutBuffer[INDEX(mWidth, x, y, 0)] = pixel.r;
			mOutBuffer[INDEX(mWidth, x, y, 1)] = pixel.g;
			mOutBuffer[INDEX(mWidth, x, y, 2)] = pixel.b;
		}
	}
	delete[] modifiedBuffer;
}

void Renderer::bloom() {
	float* modifiedBuffer = new float[mWidth * mHeight * 3];
	memcpy((void*)modifiedBuffer, (void*)mOutBuffer, mWidth * mHeight * 3 * sizeof(float));
	for (int x = 0; x < mWidth; x++) {
		for (int y = 0; y < mHeight; y++) {
			vec3 pixelColor(mOutBuffer[INDEX(mWidth, x, y, 0)], mOutBuffer[INDEX(mWidth, x, y, 1)], mOutBuffer[INDEX(mWidth, x, y, 2)]);
			if (dot(pixelColor, vec3(0.2989, 0.5870, 0.1140)) <= 0.5 )  clearPixel(x, y);
		}
	}
	blur();
	for (int x = 0; x < mWidth; x++) {
		for (int y = 0; y < mHeight; y++) {
			Color pixelColor{ mOutBuffer[INDEX(mWidth, x, y, 0)], mOutBuffer[INDEX(mWidth, x, y, 1)], mOutBuffer[INDEX(mWidth, x, y, 2)] };
			Color bloomColor{ modifiedBuffer[INDEX(mWidth, x, y, 0)], modifiedBuffer[INDEX(mWidth, x, y, 1)], modifiedBuffer[INDEX(mWidth, x, y, 2)] };
			Color resultColor = pixelColor + bloomColor;
			resultColor = Color{ (float)pow(1 - exp(-resultColor.r), 1 / 2.2), 
								 (float)pow(1 - exp(-resultColor.g), 1 / 2.2),
								 (float)pow(1 - exp(-resultColor.b), 1 / 2.2) };
			mOutBuffer[INDEX(mWidth, x, y, 0)] = resultColor.r;
			mOutBuffer[INDEX(mWidth, x, y, 1)] = resultColor.g;
			mOutBuffer[INDEX(mWidth, x, y, 2)] = resultColor.b;
		}
	}
	delete[] modifiedBuffer;
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