#pragma once
#include "Renderer.h"
#include "vec.h"
#include "mat.h"
#include "Material.h"
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
	BoundryBox mBoundryBox;

	Model() : mDrawBoundryBox(false), mDrawVertexNormals(false), mDrawFaceNormals(false) {}
	const string& getName();
	virtual void setMaterialProperties() = 0;
	virtual void setMaterialProperties(const vec4& materialProperties) = 0;
	virtual void setColor(const vec3& color) = 0;
	virtual void transform(const mat4& m , const mat4& g, bool transformWorld) = 0;
	virtual void draw(Renderer*) = 0;
};


class MeshModel : public Model {
protected :
	MeshModel() {}
	vector<vec3> mVertexPositions;
	vector<vec3> mVertexNormals;
	vector<vec3> mFaceNormals;
	vector<Material> mVertexMaterials;
	mat4 mModelTransform;
	mat4 mWorldTransform;
	mat4 mNormalTransform;

public:
	MeshModel(string fileName);
	~MeshModel(void);
	virtual void loadFile(string fileName);
	void setMaterialProperties() override;
	void setMaterialProperties(const vec4& materialProperties) override;
	void setColor(const vec3& c) override;
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
