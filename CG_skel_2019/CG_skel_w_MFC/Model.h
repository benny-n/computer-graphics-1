#pragma once
#include "Renderer.h"
#include "vec.h"
#include "mat.h"
#include <string>

using namespace std;

class Model {
protected:
	virtual ~Model() {}
	class BoundryBox {
	public:
		vector<vec3> mVertexPositions;
		vec4 mMinVec;
		vec4 mMaxVec;

		BoundryBox() : mVertexPositions(24), mMinVec(vec3(FLT_MAX)), mMaxVec(vec3(FLT_MIN)) {}
		void initVertexPositions();
		void transform(const mat4& m);
		vec4 center();
		void draw(Renderer* renderer);
	};
	string mName;

public:
	bool mDrawBoundryBox;
	bool mDrawVertexNormals;
	bool mDrawFaceNormals;
	bool mUseVisualizeSlopes;
	BoundryBox mBoundryBox;
	vec3 mColor;

	Model() : mDrawBoundryBox(false), mDrawVertexNormals(false), mDrawFaceNormals(false), mUseVisualizeSlopes(false), mColor(1) {}
	const string& getName();
	void virtual transform(const mat4& m , const mat4& g, bool transformWorld) = 0;
	void virtual draw(Renderer*) = 0;
};


class MeshModel : public Model {
protected :
	MeshModel() {}
	vector<vec3> mVertexPositions;
	vector<vec3> mVertexNormals;
	vector<vec3> mFaceNormals;
	mat4 mModelTransform;
	mat4 mWorldTransform;
	mat4 mNormalTransform;

public:
	MeshModel(string fileName);
	~MeshModel(void);
	virtual void loadFile(string fileName);
	void setColor(const vec3& c);
	void calcVertexNormals();
	void transform(const mat4& m, const mat4& g, bool transformWorld) override;
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

typedef shared_ptr<Model> ModelPtr;
