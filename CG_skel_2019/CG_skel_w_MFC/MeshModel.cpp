#include "StdAfx.h"
#include "MeshModel.h"
#include "vec.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

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
	string base_filename = fileName.substr(fileName.find_last_of("/\\") + 1);
	string::size_type const p(base_filename.find_last_of('.'));
	name = base_filename.substr(0, p);

	loadFile(fileName);
}

MeshModel::~MeshModel(void)
{
}

void MeshModel::loadFile(string fileName)
{
	ifstream ifile(fileName.c_str());
	vector<FaceIdcs> faces;
	vector<vec3> vertices;
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
			boundry_box.vmin.x = min(boundry_box.vmin.x, vertex.x);
			boundry_box.vmin.y = min(boundry_box.vmin.y, vertex.y);
			boundry_box.vmin.z = min(boundry_box.vmin.z, vertex.z);
			boundry_box.vmax.x = max(boundry_box.vmax.x, vertex.x);
			boundry_box.vmax.y = max(boundry_box.vmax.y, vertex.y);
			boundry_box.vmax.z = max(boundry_box.vmax.z, vertex.z);
			vertices.push_back(vertex);
		}
			
		else if (lineType == "f") // maybe TODO?
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
	boundry_box.initVertexPositions();

	//Vertex_positions is an array of vec3. Every three elements define a triangle in 3D.
	//If the face part of the obj is
	//f 1 2 3
	//f 1 3 4
	//Then vertex_positions should contain:
	//vertex_positions={v1,v2,v3,v1,v3,v4}

	vertex_positions = vector<vec3>(faces.size() * 3);
	// iterate through all stored faces and create triangles
	int k=0;
	for (vector<FaceIdcs>::iterator it = faces.begin(); it != faces.end(); ++it)
	{
		for (int i = 0; i < 3; i++)
		{
			vertex_positions[k++] = vertices[(*it).v[i] - 1]; 
		}
	}
}

void MeshModel::setColor(const vec3& c)
{
	color = c;
}

void MeshModel::transform(const mat4& m) {
	mat4 rotation = Translate(boundry_box.center()) * m * Translate(-boundry_box.center());
	_model_transform = rotation * _model_transform;
	boundry_box.transform(rotation);
}

void MeshModel::draw(Renderer* renderer)
{
	if (use_visualize_slopes) renderer->SetVisualizeSlopes();
	else renderer->SetColor(color);
	renderer->SetObjectMatrices(_model_transform, _normal_transform);
	renderer->DrawTriangles(&vertex_positions);
	if (draw_boundry_box) boundry_box.draw(renderer);
	//boundry_box.draw(renderer);
	//cout << _model_transform << endl;
	//cout << boundry_box.center() << endl;
}

// Prim
PrimMeshModel::~PrimMeshModel(){}

// Cube
CubeMeshModel::CubeMeshModel() {
	name = "cube";
	vertex_positions = vector<vec3>(36);

	// First Face (bottom)
		// First Triangle
	vertex_positions[0] = vec3(-1, -1, 1);
	vertex_positions[1] = vec3(-1, -1, -1);
	vertex_positions[2] = vec3(1, -1, 1);
		// Second Triangle
	vertex_positions[3] = vec3(-1, -1, -1);
	vertex_positions[4] = vec3(1, -1, -1);
	vertex_positions[5] = vec3(1, -1, 1);

	// Second Face (right)
		// First Triangle
	vertex_positions[6] = vec3(1, -1, 1);
	vertex_positions[7] = vec3(1, -1, -1);
	vertex_positions[8] = vec3(1, 1, -1);
		// Second Triangle
	vertex_positions[9] = vec3(1, -1, 1);
	vertex_positions[10] = vec3(1, 1, -1);
	vertex_positions[11] = vec3(1, 1, 1);

	// Third Face (front)
		// First Triangle
	vertex_positions[12] = vec3(-1, -1, 1);
	vertex_positions[13] = vec3(1, -1, 1);
	vertex_positions[14] = vec3(1, 1, 1);
		// Second Triangle
	vertex_positions[15] = vec3(-1, -1, 1);
	vertex_positions[16] = vec3(1, 1, 1);
	vertex_positions[17] = vec3(-1, 1, 1);

	// Fourth Face (top)
		// First Triangle
	vertex_positions[18] = vec3(-1, 1, 1);
	vertex_positions[19] = vec3(1, 1, 1);
	vertex_positions[20] = vec3(1, 1, -1);
		// Second Triangle
	vertex_positions[21] = vec3(-1, 1, -1);
	vertex_positions[22] = vec3(-1, 1, 1);
	vertex_positions[23] = vec3(1, 1, -1);

	// Fifth Face (left)
		// First Triangle
	vertex_positions[24] = vec3(-1, 1, 1);
	vertex_positions[25] = vec3(-1, -1, -1);
	vertex_positions[26] = vec3(-1, -1, 1);
		// Second Triangle
	vertex_positions[27] = vec3(-1, -1, -1);
	vertex_positions[28] = vec3(-1, 1, 1);
	vertex_positions[29] = vec3(-1, 1, -1);

	// Sixth Face (back)
		// First Triangle
	vertex_positions[30] = vec3(-1, -1, -1);
	vertex_positions[31] = vec3(-1, 1, -1);
	vertex_positions[32] = vec3(1, -1, -1);
		// Second Triangle
	vertex_positions[33] = vec3(1, -1, -1);
	vertex_positions[34] = vec3(-1, 1, -1);
	vertex_positions[35] = vec3(1, 1, -1);

	// Boundry box in this case is just the cube itself
	boundry_box.vmax = vec4(1);
	boundry_box.vmin = vec4(vec3(-1));
	boundry_box.initVertexPositions();
}

// Pyramid
PyramidMeshModel::PyramidMeshModel() {
	name = "pyramid";
	vertex_positions = vector<vec3>(18);

	// Base
		// First Triangle
	vertex_positions[0] = vec3(-1, 0, 1);
	vertex_positions[1] = vec3(-1, 0, -1);
	vertex_positions[2] = vec3(1, 0, 1);
		// Second Triangle
	vertex_positions[3] = vec3(-1, 0, -1);
	vertex_positions[4] = vec3(1, 0, -1);
	vertex_positions[5] = vec3(1, 0, 1);

	// First Face
	vertex_positions[6] = vec3(1, 0, 1);
	vertex_positions[7] = vec3(1, 0, -1);
	vertex_positions[8] = vec3(0, 2, 0);

	// Second Face
	vertex_positions[9] = vec3(1, 0, -1);
	vertex_positions[10] = vec3(-1, 0, -1);
	vertex_positions[11] = vec3(0, 2, 0);

	// Third Face
	vertex_positions[12] = vec3(-1, 0, 1);
	vertex_positions[13] = vec3(-1, 0, 1);
	vertex_positions[14] = vec3(0, 2, 0);

	// Fourth Face
	vertex_positions[15] = vec3(-1, 0, 1);
	vertex_positions[16] = vec3(1, 0, 1);
	vertex_positions[17] = vec3(0, 2, 0);

	// Boundry box
	boundry_box.vmax = vec4(vec3(1, 2, 1));
	boundry_box.vmin = vec4(vec3(-1, 0, -1));
	boundry_box.initVertexPositions();
}


