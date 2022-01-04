#pragma once
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
	virtual void setMaterialProperties(const Color& color) = 0;
	virtual void setMaterialProperties(const Material& material) = 0;
	virtual void transform(const mat4& m , const mat4& g, bool transformWorld) = 0;
	virtual void draw(const mat4&) {};
	virtual const vector<vec3>& getVertices() = 0;
	virtual const vector<vec3>& getVertexNormals() = 0;
	virtual const vector<Material>& getMaterials() = 0;
	virtual const mat4& getModelTransform() = 0;
	virtual const mat4& getWorldTransform() = 0;
	virtual const mat4& getNormalTransform() = 0;
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
	void setMaterialProperties(const Color& color) override;
	void setMaterialProperties(const Material& material) override;
	void calcVertexNormals();
	void transform(const mat4& m, const mat4& g, bool transformWorld) override;
	const vector<vec3>& getVertices();
	const vector<vec3>& getVertexNormals();
	const vector<Material>& getMaterials();
	const mat4& getModelTransform();
	const mat4& getWorldTransform();
	const mat4& getNormalTransform();
};

class PrimMeshModel : public MeshModel {
public:
	virtual ~PrimMeshModel() = 0;
	void loadFile(string fileName) override {}
};

class CubeMeshModel : public PrimMeshModel {
public:
	CubeMeshModel();
	void draw(const mat4&) override;
};

class PyramidMeshModel : public MeshModel {
public:
	PyramidMeshModel();
};

typedef shared_ptr<Model> ModelPtr;
