#pragma once
#include "scene.h"
#include "vec.h"
#include "mat.h"
#include <string>

using namespace std;

class MeshModel : public Model {
protected :
	MeshModel() : use_visualize_slopes(true) {}
	vector<vec3> vertex_positions;
	//add more attributes
	mat4 _model_transform;
	mat4 _world_transform;
	mat3 _normal_transform;
	bool use_visualize_slopes;
	vec3 color;

public:

	MeshModel(string fileName);
	~MeshModel(void);
	virtual void loadFile(string fileName);
	void setColor(const vec3& c);
	void transform(const mat4& m, bool is_rotation = false) override;
	void draw(Renderer* renderer) override;
	
};

class PrimMeshModel : public MeshModel {
public:
	virtual ~PrimMeshModel() = 0;
	void loadFile(string fileName) override {}
};

class CubeMeshModel : public PrimMeshModel {
public:
	CubeMeshModel();
};

class PyramidMeshModel : public MeshModel {
public:
	PyramidMeshModel();
};

