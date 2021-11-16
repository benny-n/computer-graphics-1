#include "StdAfx.h"
#include "Renderer.h"
#include "CG_skel_w_MFC.h"
#include "InitShader.h"
#include "GL\freeglut.h"

#define INDEX(width,x,y,c) (x+y*width)*3+c

Renderer::Renderer() : m_outBuffer(nullptr), m_width(512), m_height(512) {
	InitOpenGLRendering();
	CreateBuffers(512,512);
	SetVisualizeSlopes();
}

Renderer::Renderer(int width, int height) : m_outBuffer(nullptr), m_width(width), m_height(height) {
	InitOpenGLRendering();
	CreateBuffers(width,height);
	SetVisualizeSlopes();
}

Renderer::~Renderer(void)
{
	free(m_outBuffer);
}

vec2 Renderer::GetScreenSize()
{
	return vec2(m_width, m_height);
}

void Renderer::CreateBuffers(int width, int height)
{
	m_width=width;
	m_height=height;
	CreateOpenGLBuffer(); //Do not remove this line.
	if (m_outBuffer == nullptr) {
		RECT desktop;
		const HWND hdesktop = GetDesktopWindow();
		GetWindowRect(hdesktop, &desktop);
		m_outBuffer = (GLfloat*)malloc(sizeof(GLfloat) * (3 * desktop.right * desktop.bottom));
		SwapBuffers();
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, m_outBuffer);
	}
	glutReshapeWindow(width, height);
}

void Renderer::SetDemoBuffer()
{
	//vertical line
	for(int i=0; i<m_width; i++)
	{
		m_outBuffer[INDEX(m_width,256,i,0)]=1;	m_outBuffer[INDEX(m_width,256,i,1)]=0;	m_outBuffer[INDEX(m_width,256,i,2)]=0;

	}
	//horizontal line
	for(int i=0; i<m_width; i++)
	{
		m_outBuffer[INDEX(m_width,i,256,0)]=1;	m_outBuffer[INDEX(m_width,i,256,1)]=0;	m_outBuffer[INDEX(m_width,i,256,2)]=1;

	}

}

void Renderer::SetColor(const vec3& color)
{
	colors[0] = colors[1] = colors[2] = colors[3] = color;
}

void Renderer::SetVisualizeSlopes()
{
	colors[0] = vec3(1);
	colors[1] = vec3(1, 0, 0);
	colors[2] = vec3(0, 1, 0);
	colors[3] = vec3(0, 0, 1);
}

void Renderer::Reshape(int width, int height){
	CreateBuffers(width, height);
	//m_outBuffer = (GLfloat*)realloc(m_outBuffer, sizeof(GLfloat) * (3 * m_width * m_height));
	// this is stupid but might save our asses sometime
	//for (int i = 0; i < 10000 && m_outBuffer == nullptr; i++) m_outBuffer = (GLfloat*)realloc(m_outBuffer, sizeof(GLfloat) * (3 * m_width * m_height));
	//if (m_outBuffer == nullptr) exit(777);
	//m_width = width;
	//m_height = height;
	//glViewport(0, 0, width, height);
}

void Renderer::ColorPixel(int x, int y, const vec3& color) {
	if (x >= m_width || x < 0) return; //clip
	if (y >= m_height || y < 0) return; //clip
	m_outBuffer[INDEX(m_width, x, y, 0)] = color.x;
	m_outBuffer[INDEX(m_width, x, y, 1)] = color.y;
	m_outBuffer[INDEX(m_width, x, y, 2)] = color.z;
}

void Renderer::ClearPixel(int x, int y) {
	ColorPixel(x, y, vec3());
}

void Renderer::DrawCamera(const vec4& eye){
	mat4 project;
	project[2][2] = 0;
	const mat4 final_transformation = project * m_projection * m_cTransform;
	vec4 transformed_eye = final_transformation * eye;
	transformed_eye /= eye.w;
	vec3 final_eye = vec3(transformed_eye.x, transformed_eye.y, transformed_eye.w);
	DrawLine(final_eye.x - 5, final_eye.y, final_eye.x + 5, final_eye.y);
	DrawLine(final_eye.x, final_eye.y - 5, final_eye.x, final_eye.y + 5);
}

static void ChoosePixlesForCanonicalLine(vector<int>& ys, int x1, int y1) { // Bresenham Algorithm
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

void Renderer::DrawLine(int x1, int y1, int x2, int y2) {
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
	if (x1 > m_width || x2 < 0 || (y1 < 0 && y2 < 0) || (y1 > m_height && y2 > m_height)) return;	

	if (m==FLT_MAX) {
		y1 = max(0, y1);
		y2 = min(m_height, y2);
	}
	else if (m == 0) {
		x1 = max(0, x1);
		x2 = min(m_width, x2);
	}
	else {
		float n = y1 - m * x1;
		// clip parts of line that are not on sceen
		vector<vec2> nominees;
		// intersect y=0 at (-n/m, 0)
		float x_a = -n / m;
		if (0 <= x_a && x_a <= m_width) nominees.push_back(vec2(x_a, 0));
		// intersect x=0 at (0,n)
		if (0 <= n && n <= m_height) nominees.push_back(vec2(0, n));
		// intersect y = m_height at ((m_height - n) / m, m_height)
		float x_c = (m_height - n) / m;
		if (0 <= x_c && x_c <= m_width) nominees.push_back(vec2(x_c, m_height));
		// intersect x = m_width at (m_width, m * m_width + n)
		float y_d = m * m_width + n;
		if (0 <= y_d && y_d <= m_height) nominees.push_back(vec2(m_width, y_d));

		switch (nominees.size()) {
		case 0:
			return;
		case 1:
			//cout << nominees[0] << endl;
			x1 = x2 = nominees[0].x;
			y1 = y2 = nominees[0].y;
			break;
		default:
		
			auto find_closest_intersect = [nominees](int& x, int& y) {
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
		
			if (!(0 <= x1 && x1 <= m_width && 0 <= y1 && y1 <= m_height)) {
				find_closest_intersect(x1, y1);
			}
			if (!(0 <= x2 && x2 <= m_width && 0 <= y2 && y2 <= m_height)) {
				find_closest_intersect(x2, y2);
			}
			break;
		}
	}
	
	if (m >= 0) {
		if (m <= 1) { // Just need to move to origin
			int num_pixels = abs(x2 - x1);
			auto ys = vector<int>(num_pixels);
			ChoosePixlesForCanonicalLine(ys, x2 - x1, y2 - y1);
			for (int x = 0; x < num_pixels; x++)
			{
				ColorPixel(x + x1, ys[x] + y1, colors[0]);
			}
		}
		else { // move to origin and reflect by y=x
			int num_pixels = abs(y2 - y1);
			auto ys = vector<int>(num_pixels);
			ChoosePixlesForCanonicalLine(ys, y2 - y1, x2 - x1); // first translate, then reflect (swap x and y)
			for (int x = 0; x < num_pixels; x++)
			{
				ColorPixel(ys[x] + x1, x + y1, colors[1]); // first reflect back, then translate back
			}
		}
	}
	
	else {
		if (m >= -1) { // move to origin and reflect by x=0
			int num_pixels = abs(x2 - x1);
			auto ys = vector<int>(num_pixels);
			ChoosePixlesForCanonicalLine(ys, x2 - x1, y1 - y2); // first translate, then reflect (minus on y)
			for (int x = 0; x < num_pixels; x++)
			{
				ColorPixel(x + x1, -(ys[x]) + y1, colors[2]); // first reflect back, then translate back
			}
		}
		else { // move to origin, reflect by x=0 and then reflect by y=x
			int num_pixels = abs(y2 - y1);
			auto ys = vector<int>(num_pixels);
			ChoosePixlesForCanonicalLine(ys, y1 - y2, x2 - x1); // first translate, then reflect (minus on y) and reflect again (swap x and y)
			for (int x = 0; x < num_pixels; x++)
			{
				ColorPixel(ys[x] + x1, -x + y1, colors[3]); // first reflect back on y=x, then reflect back on x=0, then translate back
			}
		}
	}

}

void Renderer::SetCameraTransform(const mat4& cTransform) { m_cTransform = cTransform; }
void Renderer::SetProjection(const mat4& projection) { m_projection = projection;  }
void Renderer::SetObjectMatrices(const mat4& oTransform, const mat3& nTransform) {
	m_oTransform = oTransform;
	m_nTransform = nTransform;
}

mat4 Renderer::CalcFinalTransformation() {
	//int max_aspect = max(m_height / m_width, m_width / m_height);
	mat4 world_transform = mat4();
	if (m_width > m_height) world_transform = Scale(1, m_width / (float)m_height, 1);
	else  world_transform = Scale(m_height / (float)m_width, 1, 1);
	return m_projection * m_cTransform * world_transform * m_oTransform;
}

void Renderer::DrawTriangles(const vector<vec3>* vertices, const vector<vec3>* normals) {

	const mat4 final_transformation = CalcFinalTransformation();
	vec2 triangles[3];
	float r, s;

	for (int i = 0; i < vertices->size(); i+=3)
	{
		for (int j = 0; j < 3; j++)
		{
			vec4 vertex((*vertices)[i+j]);
			vertex = final_transformation * vertex;
			vertex /= vertex.w;
			r = (m_width / 2) * (vertex.x + 1);
			s = (m_height / 2) * (vertex.y + 1);
			triangles[j] = vec2(r, s);
		}
		DrawLine(triangles[0].x, triangles[0].y, triangles[1].x, triangles[1].y);
		DrawLine(triangles[1].x, triangles[1].y, triangles[2].x, triangles[2].y);
		DrawLine(triangles[2].x, triangles[2].y, triangles[0].x, triangles[0].y);
	}
}

void Renderer::DrawSquares(const vector<vec3>* vertices) {
	
	const mat4 final_transformation = CalcFinalTransformation();
	vec2 squares[4];
	float r, s;

	for (int i = 0; i < vertices->size(); i += 4)
	{
		for (int j = 0; j < 4; j++)
		{
			vec4 vertex((*vertices)[i + j]);
			vertex = final_transformation * vertex;
			vertex /= vertex.w;
			r = (m_width / 2) * (vertex.x + 1);
			s = (m_height / 2) * (vertex.y + 1);
			squares[j] = vec2(r, s);
		}
		DrawLine(squares[0].x, squares[0].y, squares[1].x, squares[1].y);
		DrawLine(squares[1].x, squares[1].y, squares[2].x, squares[2].y);
		DrawLine(squares[2].x, squares[2].y, squares[3].x, squares[3].y);
		DrawLine(squares[3].x, squares[3].y, squares[0].x, squares[0].y);
	}
}


void Renderer::ClearColorBuffer() {
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			ClearPixel(x, y);
		}
	}
}



/////////////////////////////////////////////////////
//OpenGL stuff. Don't touch.

void Renderer::InitOpenGLRendering()
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

void Renderer::CreateOpenGLBuffer()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, m_width, m_height);
}

void Renderer::SwapBuffers()
{

	int a = glGetError();
	glActiveTexture(GL_TEXTURE0);
	a = glGetError();
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	a = glGetError();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, m_outBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	a = glGetError();

	glBindVertexArray(gScreenVtc);
	a = glGetError();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	a = glGetError();
	glutSwapBuffers();
	a = glGetError();
}