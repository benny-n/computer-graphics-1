#include "StdAfx.h"
#include "Model.h"
#include "vec.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

//Boundry Box
void Model::BoundryBox::initVertexPositions()
{
	mVertexPositions = {
		mMinVec.x, mMinVec.y, mMaxVec.z,
		mMinVec.x, mMinVec.y, mMinVec.z,
		mMinVec.x, mMinVec.y, mMinVec.z,
		mMaxVec.x, mMinVec.y, mMinVec.z,
		mMaxVec.x, mMinVec.y, mMinVec.z,
		mMaxVec.x, mMinVec.y, mMaxVec.z,
		mMaxVec.x, mMinVec.y, mMaxVec.z,
		mMinVec.x, mMinVec.y, mMaxVec.z,

		mMaxVec.x, mMinVec.y, mMaxVec.z,
		mMinVec.x, mMinVec.y, mMaxVec.z,
		mMinVec.x, mMinVec.y, mMaxVec.z,
		mMinVec.x, mMaxVec.y, mMaxVec.z,
		mMinVec.x, mMaxVec.y, mMaxVec.z,
		mMaxVec.x, mMaxVec.y, mMaxVec.z,
		mMaxVec.x, mMaxVec.y, mMaxVec.z,
		mMaxVec.x, mMinVec.y, mMaxVec.z,

		mMaxVec.x, mMaxVec.y, mMaxVec.z,
		mMinVec.x, mMaxVec.y, mMaxVec.z,
		mMinVec.x, mMaxVec.y, mMaxVec.z,
		mMinVec.x, mMaxVec.y, mMinVec.z,
		mMinVec.x, mMaxVec.y, mMinVec.z,
		mMaxVec.x, mMaxVec.y, mMinVec.z,
		mMaxVec.x, mMaxVec.y, mMinVec.z,
		mMaxVec.x, mMaxVec.y, mMaxVec.z,

		mMaxVec.x, mMaxVec.y, mMinVec.z,
		mMaxVec.x, mMinVec.y, mMinVec.z,
		mMaxVec.x, mMinVec.y, mMinVec.z,
		mMinVec.x, mMinVec.y, mMinVec.z,
		mMinVec.x, mMinVec.y, mMinVec.z,
		mMinVec.x, mMaxVec.y, mMinVec.z,
		mMinVec.x, mMaxVec.y, mMinVec.z,
		mMaxVec.x, mMaxVec.y, mMinVec.z,

		mMinVec.x, mMaxVec.y, mMinVec.z,
		mMinVec.x, mMinVec.y, mMinVec.z,
		mMinVec.x, mMinVec.y, mMinVec.z,
		mMinVec.x, mMinVec.y, mMaxVec.z,
		mMinVec.x, mMinVec.y, mMaxVec.z,
		mMinVec.x, mMaxVec.y, mMaxVec.z,
		mMinVec.x, mMaxVec.y, mMaxVec.z,
		mMinVec.x, mMaxVec.y, mMinVec.z,

		mMaxVec.x, mMaxVec.y, mMaxVec.z,
		mMaxVec.x, mMaxVec.y, mMinVec.z,
		mMaxVec.x, mMaxVec.y, mMinVec.z,
		mMaxVec.x, mMinVec.y, mMinVec.z,
		mMaxVec.x, mMinVec.y, mMinVec.z,
		mMaxVec.x, mMaxVec.y, mMinVec.z,
		mMaxVec.x, mMaxVec.y, mMinVec.z,
		mMaxVec.x, mMaxVec.y, mMaxVec.z
	};

	mImmutableMinVec.x = mMinVec.x;
	mImmutableMinVec.y = mMinVec.y;

	mImmutableMaxVec.x = mMaxVec.x;
	mImmutableMaxVec.y = mMaxVec.y;

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mVertexPositions.size(), mVertexPositions.data(), GL_STATIC_DRAW);
}

void Model::BoundryBox::transform(const mat4& m)
{
	mMinVec = m * mMinVec;
	mMaxVec = m * mMaxVec;
}

vec4 Model::BoundryBox::center() {
	return (mMinVec + mMaxVec) / 2.0;
}

void Model::BoundryBox::draw(GLuint miscProgram, GLfloat transformation[]) {
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	GLuint loc = glGetAttribLocation(miscProgram, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	GLuint modelviewLoc = glGetUniformLocation(miscProgram, "modelview");
	glUniformMatrix4fv(modelviewLoc, 1, GL_TRUE, transformation);
	glDrawArrays(GL_LINES, 0, mVertexPositions.size());
}

// Model
const string& Model::getName() { return mName; }

const int Model::getNumVertices() { return mVertexPositions.size() / 3; }

void Model::setTexture() {
	mUseTexture = false;
}

void Model::setTexture(GLuint tex) {
	mUseTexture = true;
	mTexture = tex;
}

void Model::projectionOnPlain() {
	mVertexTex.clear();
	float maxX = mBoundryBox.mImmutableMaxVec.x;
	float minX = mBoundryBox.mImmutableMinVec.x;
	float maxY = mBoundryBox.mImmutableMaxVec.y;
	float minY = mBoundryBox.mImmutableMinVec.y;
	for (int i = 0; i < mVertexPositions.size(); i += 3) {
		mVertexTex.push_back((mVertexPositions[i] - minX) / (maxX - minX));
		mVertexTex.push_back((mVertexPositions[i + 1] - minY) / (maxY - minY));
	}
}

void Model::projectionOnCylinder() {
	mVertexTex.clear();
	float maxX = mBoundryBox.mImmutableMaxVec.x;
	float minX = mBoundryBox.mImmutableMinVec.x;
	float maxY = mBoundryBox.mImmutableMaxVec.y;
	float minY = mBoundryBox.mImmutableMinVec.y;
	for (int i = 0; i < mVertexPositions.size(); i += 3) {

		float theta = atan2(mVertexPositions[i + 2], mVertexPositions[i]) + M_PI;
		float h = mVertexPositions[i + 1];

		mVertexTex.push_back(theta / 2 * M_PI );
		mVertexTex.push_back((h - minY) / (maxY - minY));
	}
}

// Not ours
struct FaceIdcs
{
	int v[4];
	int vn[4];
	int vt[4];

	FaceIdcs()
	{
		for (int i=0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;
	}

	FaceIdcs(std::istream & aStream)
	{
		for (int i=0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;

		char c;
		for(int i = 0; i < 3; i++)
		{
			aStream >> std::ws >> v[i] >> std::ws;
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> std::ws;
			if (aStream.peek() == '/')
			{
				aStream >> c >> std::ws >> vn[i];
				continue;
			}
			else
				aStream >> vt[i];
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> vn[i];
		}
	}
};

vec3 vec3fFromStream(std::istream & aStream)
{
	float x, y, z;
	aStream >> x >> std::ws >> y >> std::ws >> z;
	return vec3(x, y, z);
}

vec2 vec2fFromStream(std::istream & aStream)
{
	float x, y;
	aStream >> x >> std::ws >> y;
	return vec2(x, y);
}

// MeshModel
MeshModel::MeshModel(string fileName) {
	string baseFilename = fileName.substr(fileName.find_last_of("/\\") + 1);
	string::size_type const p(baseFilename.find_last_of('.'));
	mName = baseFilename.substr(0, p);

	loadFile(fileName);
}

MeshModel::~MeshModel(void) {}

void MeshModel::loadFile(string fileName)
{
	ifstream ifile(fileName.c_str());
	vector<FaceIdcs> faces;
	vector<vec3> vertices;
	vector<vec3> vertexNormals;
	vector<vec2> vertexTex;
	float x, y, z;
	// while not end of file
	while (!ifile.eof())
	{
		// get line
		string curLine;
		getline(ifile, curLine);

		// read type of the line
		istringstream issLine(curLine);
		string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v") {
			vec3 vertex = vec3fFromStream(issLine);
			mBoundryBox.mMinVec.x = min(mBoundryBox.mMinVec.x, vertex.x);
			mBoundryBox.mMinVec.y = min(mBoundryBox.mMinVec.y, vertex.y);
			mBoundryBox.mMinVec.z = min(mBoundryBox.mMinVec.z, vertex.z);
			mBoundryBox.mMaxVec.x = max(mBoundryBox.mMaxVec.x, vertex.x);
			mBoundryBox.mMaxVec.y = max(mBoundryBox.mMaxVec.y, vertex.y);
			mBoundryBox.mMaxVec.z = max(mBoundryBox.mMaxVec.z, vertex.z);
			vertices.push_back(vertex);
		}
		else if (lineType == "vn"){
			vertexNormals.push_back(vec3fFromStream(issLine));
		}
		else if (lineType == "vt") {
			vertexTex.push_back(vec2fFromStream(issLine));
		}
		else if (lineType == "f")
			faces.push_back(issLine);
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			//cout << "Found unknown line Type \"" << lineType << "\"" << endl;
		}
	}
	mBoundryBox.initVertexPositions();

	//mVertexPositions is an array of vec3. Every three elements define a triangle in 3D.
	//If the face part of the obj is
	//f 1 2 3
	//f 1 3 4
	//Then mVertexPositions should contain:
	//mVertexPositions={v1,v2,v3,v1,v3,v4}

	mVertexMaterials = vector<Material>(faces.size() * 3);
	vector<vec3> vertexNormalPositions(faces.size() * 3);
	// iterate through all stored faces and create triangles
	int k=0;
	for (vector<FaceIdcs>::iterator it = faces.begin(); it != faces.end(); ++it) {
		for (int i = 0; i < 3; i++)
		{
			vec3 vertex = vertices[it->v[i] - 1];
			mVertexPositions.push_back(vertex.x);
			mVertexPositions.push_back(vertex.y);
			mVertexPositions.push_back(vertex.z);
			mVertexMaterials[k] = Material();
			if (!vertexNormals.empty())
				vertexNormalPositions[k] = vertexNormals[it->vn[i] - 1];
			if (!vertexTex.empty()) {
				vec2 tex = vertexTex[it->vt[i] - 1];
				mVertexTex.push_back(tex.x);
				mVertexTex.push_back(tex.y);
			}
			k++;
		}
	}
	if (vertexNormals.empty()) vertexNormalPositions = calcVertexNormals();
	if (vertexTex.empty()) projectionOnPlain();

	initVertexNormalBuffer(vertexNormalPositions);
}

void MeshModel::initVertexNormalBuffer(vector<vec3>& vertexNormals) {
	for (int i = 0; i < vertexNormals.size(); i++) {
		vec3 normal = vertexNormals[i];
		mVertexNormals.push_back(mVertexPositions[3 * i]);
		mVertexNormals.push_back(mVertexPositions[3 * i + 1]);
		mVertexNormals.push_back(mVertexPositions[3 * i + 2]);
		mVertexNormals.push_back(normal.x + mVertexPositions[3 * i]);
		mVertexNormals.push_back(normal.y + mVertexPositions[3 * i + 1]);
		mVertexNormals.push_back(normal.z + mVertexPositions[3 * i + 2]);
	}
	glGenBuffers(1, &mVertexNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mVertexNormals.size(), mVertexNormals.data(), GL_STATIC_DRAW);
}

inline static void setGlAttribute(GLuint program, const char* name, int size, int stride, int offset) {
	GLuint loc = glGetAttribLocation(program, name);
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
}

int MeshModel::initFlatBuffer(GLuint program) {
	vector<GLfloat> faceNormals = calcFaceNormals();
	vector<GLfloat> buffer;
	for (int i = 0; i < mVertexPositions.size(); i += 3) {
		// position
		buffer.push_back(mVertexPositions[i]);
		buffer.push_back(mVertexPositions[i + 1]);
		buffer.push_back(mVertexPositions[i + 2]);
		// face
		int faceIndex = (i / 9) * 6;
			// center 
		buffer.push_back(faceNormals[faceIndex]);
		buffer.push_back(faceNormals[faceIndex + 1]);
		buffer.push_back(faceNormals[faceIndex + 2]);
			// normal
		buffer.push_back(faceNormals[faceIndex + 3]);
		buffer.push_back(faceNormals[faceIndex + 4]);
		buffer.push_back(faceNormals[faceIndex + 5]);
		// material
			// ka
		int materialIndex = i / 3;
		buffer.push_back(mVertexMaterials[materialIndex].ka.r);
		buffer.push_back(mVertexMaterials[materialIndex].ka.g);
		buffer.push_back(mVertexMaterials[materialIndex].ka.b);
			// kd
		buffer.push_back(mVertexMaterials[materialIndex].kd.r);
		buffer.push_back(mVertexMaterials[materialIndex].kd.g);
		buffer.push_back(mVertexMaterials[materialIndex].kd.b);
			// ks
		buffer.push_back(mVertexMaterials[materialIndex].ks.r);
		buffer.push_back(mVertexMaterials[materialIndex].ks.g);
		buffer.push_back(mVertexMaterials[materialIndex].ks.b);
			// emission
		buffer.push_back(mVertexMaterials[materialIndex].emission.r);
		buffer.push_back(mVertexMaterials[materialIndex].emission.g);
		buffer.push_back(mVertexMaterials[materialIndex].emission.b);
			// alpha
		buffer.push_back(mVertexMaterials[materialIndex].alpha);
		// tex
		int texIndex = (i / 3) * 2;
		buffer.push_back(mVertexTex[texIndex]);
		buffer.push_back(mVertexTex[texIndex + 1]);
	}
	GLuint flatBuffer;
	glGenBuffers(1, &flatBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, flatBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buffer.size(), buffer.data(), GL_STATIC_DRAW);

	int stride = 24;
	setGlAttribute(program, "vPosition", 3, stride, 0);
	setGlAttribute(program, "faceCenter", 3, stride, 3);
	setGlAttribute(program, "faceNormal", 3, stride, 6);
	setGlAttribute(program, "ka", 3, stride, 9);
	setGlAttribute(program, "kd", 3, stride, 12);
	setGlAttribute(program, "ks", 3, stride, 15);
	setGlAttribute(program, "emission", 3, stride, 18);
	setGlAttribute(program, "alpha", 1, stride, 21);
	setGlAttribute(program, "tex", 2, stride, 22);

	return buffer.size();
}

int MeshModel::initSmoothBuffer(GLuint program) {
	vector<GLfloat> buffer;
	for (int i = 0; i < mVertexPositions.size(); i += 3) {
		// position
		buffer.push_back(mVertexPositions[i]);
		buffer.push_back(mVertexPositions[i + 1]);
		buffer.push_back(mVertexPositions[i + 2]);
		// normal
		buffer.push_back(mVertexNormals[i * 2 + 3] - mVertexPositions[i]);
		buffer.push_back(mVertexNormals[i * 2 + 4] - mVertexPositions[i + 1]);
		buffer.push_back(mVertexNormals[i * 2 + 5] - mVertexPositions[i + 2]);
		// material
			// ka
		int materialIndex = i / 3;
		buffer.push_back(mVertexMaterials[materialIndex].ka.r);
		buffer.push_back(mVertexMaterials[materialIndex].ka.g);
		buffer.push_back(mVertexMaterials[materialIndex].ka.b);
			// kd
		buffer.push_back(mVertexMaterials[materialIndex].kd.r);
		buffer.push_back(mVertexMaterials[materialIndex].kd.g);
		buffer.push_back(mVertexMaterials[materialIndex].kd.b);
			// ks
		buffer.push_back(mVertexMaterials[materialIndex].ks.r);
		buffer.push_back(mVertexMaterials[materialIndex].ks.g);
		buffer.push_back(mVertexMaterials[materialIndex].ks.b);
			// emission
		buffer.push_back(mVertexMaterials[materialIndex].emission.r);
		buffer.push_back(mVertexMaterials[materialIndex].emission.g);
		buffer.push_back(mVertexMaterials[materialIndex].emission.b);
			// alpha
		buffer.push_back(mVertexMaterials[materialIndex].alpha);
		// tex
		int texIndex = (i / 3) * 2;
		buffer.push_back(mVertexTex[texIndex]);
		buffer.push_back(mVertexTex[texIndex + 1]);
	}
	GLuint gouraudBuffer;
	glGenBuffers(1, &gouraudBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, gouraudBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * buffer.size(), buffer.data(), GL_STATIC_DRAW);

	int stride = 21;
	setGlAttribute(program, "vPosition", 3, stride, 0);
	setGlAttribute(program, "vNormal", 3, stride, 3);
	setGlAttribute(program, "ka", 3, stride, 6);
	setGlAttribute(program, "kd", 3, stride, 9);
	setGlAttribute(program, "ks", 3, stride, 12);
	setGlAttribute(program, "emission", 3, stride, 15);
	setGlAttribute(program, "alpha", 1, stride, 18);
	setGlAttribute(program, "tex", 2, stride, 19);

	GLuint normalviewLoc = glGetUniformLocation(program, "normalview");
	mat4 normalTransform = mWorldTransform * mNormalTransform;
	GLfloat normalView[16];
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			normalView[i * 4 + j] = normalTransform[i][j];
		}
	}
	glUniformMatrix4fv(normalviewLoc, 1, GL_TRUE, normalView);

	return buffer.size();
}

int MeshModel::initShaderBuffer(RasterizerPtr rasterizer) {
	GLuint program = rasterizer->getActiveProgram();
	switch (rasterizer->getShaderType()) {
		case ShaderType::Flat:
			return initFlatBuffer(program);
		case ShaderType::Gouraud:
			return initSmoothBuffer(program);
		case ShaderType::Phong:
			return initSmoothBuffer(program);
			break;
	}
}

const Material& MeshModel::getMaterial()
{
	return mVertexMaterials[0];
}

void MeshModel::growVertex(int vertexIndex) {
	for (int i = 3 * vertexIndex; i < 3 * vertexIndex + 3; i++) {
		mVertexPositions[i] *= 1.05;
	}
}

void MeshModel::draw(RasterizerPtr rasterizer, const mat4& from3dTo2d) {
	GLuint program = rasterizer->getActiveProgram();
	GLuint miscProgram = rasterizer->getMiscProgram();
	glUseProgram(program);
	int bufferSize = initShaderBuffer(rasterizer);
	GLuint modelviewLoc = glGetUniformLocation(program, "modelview");
	mat4 finalTransform = from3dTo2d * mWorldTransform * mModelTransform;
	GLfloat modelView[16];
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			modelView[i * 4 + j] = finalTransform[i][j];
		}
	}
	glUniformMatrix4fv(modelviewLoc, 1, GL_TRUE, modelView);
	GLuint useTexture = glGetUniformLocation(program, "useTex");
	glUniform1i(useTexture, mUseTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glDrawArrays(GL_TRIANGLES, 0, bufferSize);
	if (mDrawBoundryBox || mDrawVertexNormals || mDrawFaceNormals) {
		glUseProgram(miscProgram);
		if (mDrawBoundryBox) mBoundryBox.draw(miscProgram, modelView);
		if (mDrawVertexNormals) drawVertexNormals(miscProgram, finalTransform);
		if (mDrawFaceNormals) drawFaceNormals(miscProgram, from3dTo2d);
	}
}

void MeshModel::drawVertexNormals(GLuint miscProgram, const mat4& finalTransform) {
	glBindBuffer(GL_ARRAY_BUFFER, mVertexNormalBuffer);
	GLuint loc = glGetAttribLocation(miscProgram, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	GLuint modelviewLoc = glGetUniformLocation(miscProgram, "modelview");
	glUniformMatrix4fv(modelviewLoc, 1, GL_TRUE, finalTransform);
	glDrawArrays(GL_LINES, 0, mVertexNormals.size());
}

void MeshModel::drawFaceNormals(GLuint miscProgram, const mat4& from3dTo2d) {
	GLuint faceNormalBuffer;
	vector<GLfloat> faceNormals = calcFaceNormals();
	glGenBuffers(1, &faceNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, faceNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * faceNormals.size(), faceNormals.data(), GL_STATIC_DRAW);
	GLuint loc = glGetAttribLocation(miscProgram, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	GLuint modelviewLoc = glGetUniformLocation(miscProgram, "modelview");
	glUniformMatrix4fv(modelviewLoc, 1, GL_TRUE, from3dTo2d);
	glDrawArrays(GL_LINES, 0, faceNormals.size());
}

void MeshModel::setMaterialProperties() {
	for (int i = 0; i < mVertexMaterials.size(); i++) {
		mVertexMaterials[i].ka.r = (float)rand() / RAND_MAX;
		mVertexMaterials[i].ka.g = (float)rand() / RAND_MAX;
		mVertexMaterials[i].ka.b = (float)rand() / RAND_MAX;
		mVertexMaterials[i].kd.r = (float)rand() / RAND_MAX;
		mVertexMaterials[i].kd.g = (float)rand() / RAND_MAX;
		mVertexMaterials[i].kd.b = (float)rand() / RAND_MAX;
		mVertexMaterials[i].ks.r = (float)rand() / RAND_MAX;
		mVertexMaterials[i].ks.g = (float)rand() / RAND_MAX;
		mVertexMaterials[i].ks.b = (float)rand() / RAND_MAX;
		mVertexMaterials[i].emission.r = (float)rand() / RAND_MAX;
		mVertexMaterials[i].emission.g = (float)rand() / RAND_MAX;
		mVertexMaterials[i].emission.b = (float)rand() / RAND_MAX;
		mVertexMaterials[i].alpha = (float)rand() / RAND_MAX;
	}
}

void MeshModel::setMaterialProperties(const Color& color) {
	for (int i = 0; i < mVertexMaterials.size(); i++) {
		mVertexMaterials[i].ka = color;
		mVertexMaterials[i].kd = color;
		mVertexMaterials[i].ks = color;
	}
}

void MeshModel::setMaterialProperties(const Material& material) {
	for (int i = 0; i < mVertexMaterials.size(); i++) {
		mVertexMaterials[i] = material;
	}		
}

void MeshModel::setMaterialProperties(int index, const Material& material) {
	mVertexMaterials[index] = material;
}

void MeshModel::transform(const mat4& m, const mat4& g, bool transformWorld) {
	mBoundryBox.transform(m);
	if (transformWorld) {
		mWorldTransform = m * mWorldTransform;
	}
	else {
		mModelTransform = m * mModelTransform;
		mNormalTransform = g * mNormalTransform;
	}
}

//for (int i = 0; i < mVertexPositions.size(); i += 3) {
//	vertexPositions[i / 3] = vec3(mVertexPositions[i], mVertexPositions[i + 1], mVertexPositions[i + 2]);
//}

vector<vec3> MeshModel::calcVertexNormals(){
	map<vec3, vector<int>> vertexIndices;
	vector<vec3> vertexPositions(mVertexPositions.size() / 3);
	vector<vec3> vertexNormals(mVertexPositions.size() / 3);
	for (int i = 0; i < mVertexPositions.size(); i += 3) {
		vertexPositions[i / 3] = vec3(mVertexPositions[i], mVertexPositions[i + 1], mVertexPositions[i + 2]);
	}

	for (int i = 0; i < vertexPositions.size(); i++) vertexIndices[vertexPositions[i]].push_back(i);
	for (int i = 0; i < vertexPositions.size(); i += 3) {
		const vec3 v1 = vertexPositions[i];
		const vec3 v2 = vertexPositions[i + 1];
		const vec3 v3 = vertexPositions[i + 2];
		const vec3 vi = v2 - v1;
		const vec3 vj = v3 - v1;
		vec3 normal = vec3(normalize(cross(vi, vj)));
		float area = length((cross(vi, vj)));
		normal *= area;
		for each (int index in vertexIndices[v1]) vertexNormals[index] += normal;
		for each (int index in vertexIndices[v2]) vertexNormals[index] += normal;
		for each (int index in vertexIndices[v3]) vertexNormals[index] += normal;
	}
	for (int i = 0; i < vertexNormals.size(); i++) vertexNormals[i] = normalize(vertexNormals[i]);

	return vertexNormals;
}

vector<GLfloat> MeshModel::calcFaceNormals()
{
	vector<GLfloat> faceNormals;
	mat4 modelTranform = mWorldTransform * mModelTransform;
	mat4 normalTransform = mWorldTransform * mNormalTransform;
	for (int i = 0; i < mVertexPositions.size(); i += 9) {
		const vec3 v1(mVertexPositions[i], mVertexPositions[i + 1], mVertexPositions[i + 2]);
		const vec3 v2(mVertexPositions[i + 3], mVertexPositions[i + 4], mVertexPositions[i + 5]);
		const vec3 v3(mVertexPositions[i + 6], mVertexPositions[i + 7], mVertexPositions[i + 8]);
		const vec3 vi = v2 - v1;
		const vec3 vj = v3 - v1;
		vec3 center = (v1 + v2 + v3) / 3;
		center = vec3FromVec4(modelTranform * vec4(center));
		vec3 normal = vec3(cross(vi, vj));
		normal = vec3FromVec4(normalTransform * vec4(normal));
		normal = center + normalize(normal);
		faceNormals.push_back(center.x);
		faceNormals.push_back(center.y);
		faceNormals.push_back(center.z);
		faceNormals.push_back(normal.x);
		faceNormals.push_back(normal.y);
		faceNormals.push_back(normal.z);
	}

	return faceNormals;
}

// Prim
PrimMeshModel::~PrimMeshModel(){}

// Cube
CubeMeshModel::CubeMeshModel() {
	mName = "cube";
	mVertexMaterials = vector<Material>(36);

	mVertexPositions = {
		-1, -1, 1,
		-1, -1, -1,
		1, -1, 1,
		-1, -1, -1,
		1, -1, -1,
		1, -1, 1,
		1, -1, 1,
		1, -1, -1,
		1, 1, -1,
		1, -1, 1,
		1, 1, -1,
		1, 1, 1,
		-1, -1, 1,
		1, -1, 1,
		1, 1, 1,
		-1, -1, 1,
		1, 1, 1,
		-1, 1, 1,
		-1, 1, 1,
		1, 1, 1,
		1, 1, -1,
		-1, 1, -1,
		-1, 1, 1,
		1, 1, -1,
		-1, 1, 1,
		-1, -1, -1,
		-1, -1, 1,
		-1, -1, -1,
		-1, 1, 1,
		-1, 1, -1,
		-1, -1, -1,
		-1, 1, -1,
		1, -1, -1,
		1, -1, -1,
		-1, 1, -1,
		1, 1, -1
	};

	// Boundry box in this case is just the cube itself
	mBoundryBox.mMaxVec = vec4(1);
	mBoundryBox.mMinVec = vec4(vec3(-1));
	mBoundryBox.initVertexPositions();
	vector<vec3> vertexNormals = calcVertexNormals();
	projectionOnPlain();

	initVertexNormalBuffer(vertexNormals);
}

// Pyramid
PyramidMeshModel::PyramidMeshModel() {
	mName = "pyramid";
	mVertexMaterials = vector<Material>(18);

	mVertexPositions = {
		-1, 0, 1,
		-1, 0, -1,
		1, 0, 1,
		-1, 0, -1,
		1, 0, -1,
		1, 0, 1,
		1, 0, 1,
		1, 0, -1,
		0, 2, 0,
		1, 0, -1,
		-1, 0, -1,
		0, 2, 0,
		-1, 0, -1,
		-1, 0, 1,
		0, 2, 0,
		-1, 0, 1,
		1, 0, 1,
		0, 2, 0
	};

	// Boundry box
	mBoundryBox.mMaxVec = vec4(vec3(1, 2, 1));
	mBoundryBox.mMinVec = vec4(vec3(-1, 0, -1));
	mBoundryBox.initVertexPositions();
	vector<vec3> vertexNormals = calcVertexNormals();
	projectionOnPlain();

	initVertexNormalBuffer(vertexNormals);
}
