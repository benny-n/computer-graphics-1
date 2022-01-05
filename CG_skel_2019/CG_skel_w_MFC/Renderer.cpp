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

const mat4 Renderer::from3dTo2d() { return mProjection * mCameraTransform * mAspectRatioTransform; }

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

void Renderer::reshape(int width, int height){ 
	createBuffers(width, height); 
	if (mWidth > mHeight) mAspectRatioTransform = scale(1, mWidth / (float)mHeight, 1);
	else  mAspectRatioTransform = scale(mHeight / (float)mWidth, 1, 1);
}

void Renderer::setCameraTransform(const mat4& cTransform) { mCameraTransform = cTransform; }
void Renderer::setProjection(const mat4& projection) { mProjection = projection;  }
void Renderer::setObjectMatrices(const ModelPtr model) {
	mObjectTransform = model->getModelTransform();
	mNormalTransform = model->getNormalTransform();
	mWorldTransform = model->getWorldTransform();
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
			//if (dot(pixelColor, vec3(0.2126, 0.7152, 0.0722)) <= 1 )  clearPixel(x, y);
		}
	}
	blur();
	blur();
	blur();
	blur();
	blur();
	for (int x = 0; x < mWidth; x++) {
		for (int y = 0; y < mHeight; y++) {
			Color bloomColor{ mOutBuffer[INDEX(mWidth, x, y, 0)], mOutBuffer[INDEX(mWidth, x, y, 1)], mOutBuffer[INDEX(mWidth, x, y, 2)] };
			Color pixelColor{ modifiedBuffer[INDEX(mWidth, x, y, 0)], modifiedBuffer[INDEX(mWidth, x, y, 1)], modifiedBuffer[INDEX(mWidth, x, y, 2)] };
			Color resultColor;
			if (bloomColor != Color{ 0,0,0 }) {
				resultColor = pixelColor + bloomColor;
				resultColor = Color{ (float)pow(1 - exp(-resultColor.r), 1 / 2.2), 
									 (float)pow(1 - exp(-resultColor.g), 1 / 2.2),
									 (float)pow(1 - exp(-resultColor.b), 1 / 2.2) };
			}
			else resultColor = pixelColor;

			mOutBuffer[INDEX(mWidth, x, y, 0)] = resultColor.r;
			mOutBuffer[INDEX(mWidth, x, y, 1)] = resultColor.g;
			mOutBuffer[INDEX(mWidth, x, y, 2)] = resultColor.b;
		}
	}
	delete[] modifiedBuffer;
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
	for (int i = 0; i < 3 * mWidth * mHeight; i++)
		CLAMP(mOutBuffer[i]);

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