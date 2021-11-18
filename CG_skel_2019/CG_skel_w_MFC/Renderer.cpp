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

void Renderer::reshape(int width, int height){ createBuffers(width, height); }

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
	mat4 project;
	project[2][2] = 0;
	const mat4 finalTransformation = project * mProjection * mCameraTransform;
	vec4 transformedEye = (finalTransformation * eye) / eye.w;
	vec3 finalEye = vec3(transformedEye.x, transformedEye.y, transformedEye.w);
	drawLine(finalEye.x - 5, finalEye.y, finalEye.x + 5, finalEye.y);
	drawLine(finalEye.x, finalEye.y - 5, finalEye.x, finalEye.y + 5);
}

static void choosePixlesForCanonicalLine(vector<int>& ys, int x1, int y1) { // Bresenham Algorithm
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
	if (x1 > mWidth || x2 < 0 || (y1 < 0 && y2 < 0) || (y1 > mHeight && y2 > mHeight)) return;	

	if (m==FLT_MAX) {
		y1 = max(0, y1);
		y2 = min(mHeight, y2);
	}
	else if (m == 0) {
		x1 = max(0, x1);
		x2 = min(mWidth, x2);
	}
	else {
		float n = y1 - m * x1;
		// clip parts of line that are not on sceen
		vector<vec2> nominees;
		// intersect y=0 at (-n/m, 0)
		float x_a = -n / m;
		if (0 <= x_a && x_a <= mWidth) nominees.push_back(vec2(x_a, 0));
		// intersect x=0 at (0,n)
		if (0 <= n && n <= mHeight) nominees.push_back(vec2(0, n));
		// intersect y = mHeight at ((mHeight - n) / m, mHeight)
		float x_c = (mHeight - n) / m;
		if (0 <= x_c && x_c <= mWidth) nominees.push_back(vec2(x_c, mHeight));
		// intersect x = mWidth at (mWidth, m * mWidth + n)
		float y_d = m * mWidth + n;
		if (0 <= y_d && y_d <= mHeight) nominees.push_back(vec2(mWidth, y_d));

		switch (nominees.size()) {
		case 0:
			return;
		case 1:
			//cout << nominees[0] << endl;
			x1 = x2 = nominees[0].x;
			y1 = y2 = nominees[0].y;
			break;
		default:
		
			auto findClosestIntersect = [nominees](int& x, int& y) {
				float dx, dy;
				vector<float> distances = { FLT_MAX,FLT_MAX, FLT_MAX, FLT_MAX };
				for (int i = 0; i < nominees.size(); i++)
				{
					dx = nominees[i].x - x;
					dy = nominees[i].y - y;
					distances[i] = dx * dx + dy * dy;
				}
				// find position of max element in nominees
				int i = distance(distances.begin(), min_element(distances.begin(), distances.end()));
				x = nominees[i].x;
				y = nominees[i].y;
			};
		
			if (!(0 <= x1 && x1 <= mWidth && 0 <= y1 && y1 <= mHeight)) {
				findClosestIntersect(x1, y1);
			}
			if (!(0 <= x2 && x2 <= mWidth && 0 <= y2 && y2 <= mHeight)) {
				findClosestIntersect(x2, y2);
			}
			break;
		}
	}
	
	if (m >= 0) {
		if (m <= 1) { // Just need to move to origin
			int numPixels = abs(x2 - x1);
			auto ys = vector<int>(numPixels);
			choosePixlesForCanonicalLine(ys, x2 - x1, y2 - y1);
			for (int x = 0; x < numPixels; x++)
			{
				colorPixel(x + x1, ys[x] + y1, mColors[0]);
			}
		}
		else { // move to origin and reflect by y=x
			int numPixels = abs(y2 - y1);
			auto ys = vector<int>(numPixels);
			choosePixlesForCanonicalLine(ys, y2 - y1, x2 - x1); // first translate, then reflect (swap x and y)
			for (int x = 0; x < numPixels; x++)
			{
				colorPixel(ys[x] + x1, x + y1, mColors[1]); // first reflect back, then translate back
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
				colorPixel(x + x1, -(ys[x]) + y1, mColors[2]); // first reflect back, then translate back
			}
		}
		else { // move to origin, reflect by x=0 and then reflect by y=x
			int numPixels = abs(y2 - y1);
			auto ys = vector<int>(numPixels);
			choosePixlesForCanonicalLine(ys, y1 - y2, x2 - x1); // first translate, then reflect (minus on y) and reflect again (swap x and y)
			for (int x = 0; x < numPixels; x++)
			{
				colorPixel(ys[x] + x1, -x + y1, mColors[3]); // first reflect back on y=x, then reflect back on x=0, then translate back
			}
		}
	}

}

void Renderer::setCameraTransform(const mat4& cTransform) { mCameraTransform = cTransform; }
void Renderer::setProjection(const mat4& projection) { mProjection = projection;  }
void Renderer::setObjectMatrices(const mat4& oTransform, const mat3& nTransform) {
	mObjectTransform = oTransform;
	mNormalTransform = nTransform;
}

mat4 Renderer::calcFinalTransformation() {
	mat4 aspectRatioTransform = mat4();
	if (mWidth > mHeight) aspectRatioTransform = scale(1, mWidth / (float)mHeight, 1);
	else  aspectRatioTransform = scale(mHeight / (float)mWidth, 1, 1);
	return mProjection * mCameraTransform * aspectRatioTransform * mObjectTransform;
}

void Renderer::drawTriangles(const vector<vec3>* vertices, const vector<vec3>* normals) {
	const mat4 finalTransformation = calcFinalTransformation();
	vec2 triangles[3];
	float r, s;

	for (int i = 0; i < vertices->size(); i+=3)
	{
		for (int j = 0; j < 3; j++)
		{
			vec4 vertex((*vertices)[i+j]);
			vertex = finalTransformation * vertex;
			vertex /= vertex.w;
			r = (mWidth / 2) * (vertex.x + 1);
			s = (mHeight / 2) * (vertex.y + 1);
			triangles[j] = vec2(r, s);
		}
		drawLine(triangles[0].x, triangles[0].y, triangles[1].x, triangles[1].y);
		drawLine(triangles[1].x, triangles[1].y, triangles[2].x, triangles[2].y);
		drawLine(triangles[2].x, triangles[2].y, triangles[0].x, triangles[0].y);
	}
}

void Renderer::drawSquares(const vector<vec3>* vertices) {
	const mat4 finalTransformation = calcFinalTransformation();
	vec2 squares[4];
	float r, s;

	for (int i = 0; i < vertices->size(); i += 4)
	{
		for (int j = 0; j < 4; j++)
		{
			vec4 vertex((*vertices)[i + j]);
			vertex = finalTransformation * vertex;
			vertex /= vertex.w;
			r = (mWidth / 2) * (vertex.x + 1);
			s = (mHeight / 2) * (vertex.y + 1);
			squares[j] = vec2(r, s);
		}
		drawLine(squares[0].x, squares[0].y, squares[1].x, squares[1].y);
		drawLine(squares[1].x, squares[1].y, squares[2].x, squares[2].y);
		drawLine(squares[2].x, squares[2].y, squares[3].x, squares[3].y);
		drawLine(squares[3].x, squares[3].y, squares[0].x, squares[0].y);
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