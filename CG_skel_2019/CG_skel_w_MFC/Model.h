#pragma once
#include "Renderer.h"
#include "vec.h"
#include "mat.h"
#include <string>

using namespace std;

class Model {

public:
	bool mDrawBoundryBox;
	bool mDrawVertexNormals;
	bool mDrawFaceNormals;
	bool mUseVisualizeSlopes;
	vec3 mColor;

	Model() : mDrawBoundryBox(false), mDrawVertexNormals(false), mDrawFaceNormals(false), mUseVisualizeSlopes(false), mColor(1) {}
	const string& getName();
	void virtual transform(const mat4& m) = 0;
	void virtual transformWorld(const mat4& m) = 0;
	void virtual draw(Renderer*) = 0;

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
	BoundryBox mBoundryBox;
	string mName;
};


class MeshModel : public Model {
protected :
	MeshModel() {}
	vector<vec3> mVertexPositions;
	mat4 mModelTransform;
	mat4 mWorldTransform;
	mat3 mNormalTransform;

public:
	MeshModel(string fileName);
	~MeshModel(void);
	virtual void loadFile(string fileName);
	void setColor(const vec3& c);
	void transform(const mat4& m) override;
	void transformWorld(const mat4& m) override;
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
