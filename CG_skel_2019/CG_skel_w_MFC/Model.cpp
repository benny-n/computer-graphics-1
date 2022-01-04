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
	// First Face (bottom)
	mVertexPositions[0] = vec3(mMinVec.x, mMinVec.y, mMaxVec.z);
	mVertexPositions[1] = vec3(mMinVec.x, mMinVec.y, mMinVec.z);
	mVertexPositions[2] = vec3(mMaxVec.x, mMinVec.y, mMinVec.z);
	mVertexPositions[3] = vec3(mMaxVec.x, mMinVec.y, mMaxVec.z);
	// Second Face (front)
	mVertexPositions[4] = vec3(mMaxVec.x, mMinVec.y, mMaxVec.z);
	mVertexPositions[5] = vec3(mMinVec.x, mMinVec.y, mMaxVec.z);
	mVertexPositions[6] = vec3(mMinVec.x, mMaxVec.y, mMaxVec.z);
	mVertexPositions[7] = vec3(mMaxVec.x, mMaxVec.y, mMaxVec.z);
	// Third Face (top)
	mVertexPositions[8] = vec3(mMaxVec.x, mMaxVec.y, mMaxVec.z);
	mVertexPositions[9] = vec3(mMinVec.x, mMaxVec.y, mMaxVec.z);
	mVertexPositions[10] = vec3(mMinVec.x, mMaxVec.y, mMinVec.z);
	mVertexPositions[11] = vec3(mMaxVec.x, mMaxVec.y, mMinVec.z);
	// Fourth Face (back)
	mVertexPositions[12] = vec3(mMaxVec.x, mMaxVec.y, mMinVec.z);
	mVertexPositions[13] = vec3(mMaxVec.x, mMinVec.y, mMinVec.z);
	mVertexPositions[14] = vec3(mMinVec.x, mMinVec.y, mMinVec.z);
	mVertexPositions[15] = vec3(mMinVec.x, mMaxVec.y, mMinVec.z);
	// Fifth Face (left)
	mVertexPositions[16] = vec3(mMinVec.x, mMaxVec.y, mMinVec.z);
	mVertexPositions[17] = vec3(mMinVec.x, mMinVec.y, mMinVec.z);
	mVertexPositions[18] = vec3(mMinVec.x, mMinVec.y, mMaxVec.z);
	mVertexPositions[19] = vec3(mMinVec.x, mMaxVec.y, mMaxVec.z);
	// Sixth Face (right)
	mVertexPositions[20] = vec3(mMaxVec.x, mMaxVec.y, mMaxVec.z);
	mVertexPositions[21] = vec3(mMaxVec.x, mMaxVec.y, mMinVec.z);
	mVertexPositions[22] = vec3(mMaxVec.x, mMinVec.y, mMinVec.z);
	mVertexPositions[23] = vec3(mMaxVec.x, mMaxVec.y, mMinVec.z);
}

void Model::BoundryBox::transform(const mat4& m)
{
	mMinVec = m * mMinVec;
	mMaxVec = m * mMaxVec;
}

vec4 Model::BoundryBox::center() {
	return (mMinVec + mMaxVec) / 2.0;
}

// Model
const string& Model::getName() {
	return mName;
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

	mVertexPositions = vector<vec3>(faces.size() * 3);
	mVertexNormals = vector<vec3>(faces.size() * 3);
	mVertexMaterials = vector<Material>(faces.size() * 3);
	// iterate through all stored faces and create triangles
	int k=0;
	for (vector<FaceIdcs>::iterator it = faces.begin(); it != faces.end(); ++it) {
		for (int i = 0; i < 3; i++)
		{
			mVertexPositions[k] = vertices[(*it).v[i] - 1];
			mVertexMaterials[k] = Material();
			if (!vertexNormals.empty())
				mVertexNormals[k] = vertexNormals[(*it).vn[i] - 1];
			k++;
		}
	}
	if (vertexNormals.empty()) calcVertexNormals();
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
		mVertexMaterials[i].emission = color;
	}
}

void MeshModel::setMaterialProperties(const Material& material) {
	for (int i = 0; i < mVertexMaterials.size(); i++) {
		mVertexMaterials[i] = material;
	}		
}

void MeshModel::calcVertexNormals(){
	map<vec3, vector<int>> vertexIndeces;
	for (int i = 0; i < mVertexPositions.size(); i++) vertexIndeces[mVertexPositions[i]].push_back(i);
	for (int i = 0; i < mVertexPositions.size(); i += 3) {
		const vec3 v1 = mVertexPositions[i];
		const vec3 v2 = mVertexPositions[i + 1];
		const vec3 v3 = mVertexPositions[i + 2];
		const vec3 vi = v2 - v1;
		const vec3 vj = v3 - v1;
		vec3 normal = vec3(normalize(cross(vi, vj)));
		float area = length((cross(vi, vj)));
		normal *= area;
		for each (int index in vertexIndeces[v1]) mVertexNormals[index] += normal;
		for each (int index in vertexIndeces[v2]) mVertexNormals[index] += normal;
		for each (int index in vertexIndeces[v3]) mVertexNormals[index] += normal;
	}
	for (int i = 0; i < mVertexNormals.size(); i++) mVertexNormals[i] = normalize(mVertexNormals[i]);
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

const vector<vec3>& MeshModel::getVertices() { return mVertexPositions; }

const vector<vec3>& MeshModel::getVertexNormals() { return mVertexNormals; }

const vector<Material>& MeshModel::getMaterials() { return mVertexMaterials; }

const mat4& MeshModel::getModelTransform() { return mModelTransform; }

const mat4& MeshModel::getWorldTransform() { return mWorldTransform; }

const mat4& MeshModel::getNormalTransform() { return mNormalTransform; }

// Prim
PrimMeshModel::~PrimMeshModel(){}

// Cube
CubeMeshModel::CubeMeshModel() {
	mName = "cube";
	mVertexPositions = vector<vec3>(36);
	mVertexNormals = vector<vec3>(36);
	mVertexMaterials = vector<Material>(36);

	// First Face (bottom)
		// First Triangle
	mVertexPositions[0] = vec3(-1, -1, 1);
	mVertexPositions[1] = vec3(-1, -1, -1);
	mVertexPositions[2] = vec3(1, -1, 1);
		// Second Triangle
	mVertexPositions[3] = vec3(-1, -1, -1);
	mVertexPositions[4] = vec3(1, -1, -1);
	mVertexPositions[5] = vec3(1, -1, 1);

	// Second Face (right)
		// First Triangle
	mVertexPositions[6] = vec3(1, -1, 1);
	mVertexPositions[7] = vec3(1, -1, -1);
	mVertexPositions[8] = vec3(1, 1, -1);
		// Second Triangle
	mVertexPositions[9] = vec3(1, -1, 1);
	mVertexPositions[10] = vec3(1, 1, -1);
	mVertexPositions[11] = vec3(1, 1, 1);

	// Third Face (front)
		// First Triangle
	mVertexPositions[12] = vec3(-1, -1, 1);
	mVertexPositions[13] = vec3(1, -1, 1);
	mVertexPositions[14] = vec3(1, 1, 1);
		// Second Triangle
	mVertexPositions[15] = vec3(-1, -1, 1);
	mVertexPositions[16] = vec3(1, 1, 1);
	mVertexPositions[17] = vec3(-1, 1, 1);

	// Fourth Face (top)
		// First Triangle
	mVertexPositions[18] = vec3(-1, 1, 1);
	mVertexPositions[19] = vec3(1, 1, 1);
	mVertexPositions[20] = vec3(1, 1, -1);
		// Second Triangle
	mVertexPositions[21] = vec3(-1, 1, -1);
	mVertexPositions[22] = vec3(-1, 1, 1);
	mVertexPositions[23] = vec3(1, 1, -1);

	// Fifth Face (left)
		// First Triangle
	mVertexPositions[24] = vec3(-1, 1, 1);
	mVertexPositions[25] = vec3(-1, -1, -1);
	mVertexPositions[26] = vec3(-1, -1, 1);
		// Second Triangle
	mVertexPositions[27] = vec3(-1, -1, -1);
	mVertexPositions[28] = vec3(-1, 1, 1);
	mVertexPositions[29] = vec3(-1, 1, -1);

	// Sixth Face (back)
		// First Triangle
	mVertexPositions[30] = vec3(-1, -1, -1);
	mVertexPositions[31] = vec3(-1, 1, -1);
	mVertexPositions[32] = vec3(1, -1, -1);
		// Second Triangle
	mVertexPositions[33] = vec3(1, -1, -1);
	mVertexPositions[34] = vec3(-1, 1, -1);
	mVertexPositions[35] = vec3(1, 1, -1);

	// Boundry box in this case is just the cube itself
	mBoundryBox.mMaxVec = vec4(1);
	mBoundryBox.mMinVec = vec4(vec3(-1));
	mBoundryBox.initVertexPositions();
	calcVertexNormals();
}

void CubeMeshModel::draw(const mat4& from3dTo2d) {
	GLuint program = InitShader("flat_vshader.glsl", "flat_fshader.glsl");
	GLfloat points[108];
	for (size_t i = 0; i < 36; i++) {
		points[3 * i] = mVertexPositions[i].x;
		points[3 * i + 1] = mVertexPositions[i].y;
		points[3 * i + 2] = mVertexPositions[i].z;
	}
	glUseProgram(program);
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	GLuint modelviewLoc = glGetUniformLocation(program, "modelview");
	mat4 finalTransform = from3dTo2d * mWorldTransform * mModelTransform;
	GLfloat modelView[16];
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			modelView[i * 4 + j] = finalTransform[i][j];
		}
	}
	glUniformMatrix4fv(modelviewLoc, 1, GL_TRUE, modelView);
	glDrawArrays(GL_TRIANGLES, 0, 108);
}

// Pyramid
PyramidMeshModel::PyramidMeshModel() {
	mName = "pyramid";
	mVertexPositions = vector<vec3>(18);
	mVertexNormals = vector<vec3>(18);
	mVertexMaterials = vector<Material>(18);

	// Base
		// First Triangle
	mVertexPositions[0] = vec3(-1, 0, 1);
	mVertexPositions[1] = vec3(-1, 0, -1);
	mVertexPositions[2] = vec3(1, 0, 1);
		// Second Triangle
	mVertexPositions[3] = vec3(-1, 0, -1);
	mVertexPositions[4] = vec3(1, 0, -1);
	mVertexPositions[5] = vec3(1, 0, 1);

	// First Face
	mVertexPositions[6] = vec3(1, 0, 1);
	mVertexPositions[7] = vec3(1, 0, -1);
	mVertexPositions[8] = vec3(0, 2, 0);

	// Second Face
	mVertexPositions[9] = vec3(1, 0, -1);
	mVertexPositions[10] = vec3(-1, 0, -1);
	mVertexPositions[11] = vec3(0, 2, 0);

	// Third Face
	mVertexPositions[12] = vec3(-1, 0, -1);
	mVertexPositions[13] = vec3(-1, 0, 1);
	mVertexPositions[14] = vec3(0, 2, 0);

	// Fourth Face
	mVertexPositions[15] = vec3(-1, 0, 1);
	mVertexPositions[16] = vec3(1, 0, 1);
	mVertexPositions[17] = vec3(0, 2, 0);

	// Boundry box
	mBoundryBox.mMaxVec = vec4(vec3(1, 2, 1));
	mBoundryBox.mMinVec = vec4(vec3(-1, 0, -1));
	mBoundryBox.initVertexPositions();
	calcVertexNormals();
}
