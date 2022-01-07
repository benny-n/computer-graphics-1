#pragma once
#include "vec.h"
#include "mat.h"
#include "Material.h"
#include "Rasterizer.h"
#include <string>

using namespace std;

class Model {
protected:
	virtual ~Model() {}
	class BoundryBox {
		vector<GLfloat> mVertexPositions;
		GLuint mVertexBuffer;

	public:
		vec4 mMinVec;
		vec4 mMaxVec;

		BoundryBox() : mVertexPositions(72), mMinVec(vec3(FLT_MAX)), mMaxVec(vec3(FLT_MIN)) {}
		void initVertexPositions();
		void transform(const mat4& m);
		vec4 center();
		void draw(GLuint miscProgram, GLfloat transformation[]);
	};
	string mName;
	vector<GLfloat> mVertexPositions;
	vector<GLfloat> mVertexNormals;
	GLuint mVertexNormalBuffer;
	vector<GLfloat> mFaceNormals;
	GLuint mFaceNormalBuffer;

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
	virtual void draw(RasterizerPtr rasterizer, const mat4& from3dTo2d) = 0;
	virtual const vector<Material>& getMaterials() = 0;
	virtual const mat4& getModelTransform() = 0;
	virtual const mat4& getWorldTransform() = 0;
	virtual const mat4& getNormalTransform() = 0;
};


class MeshModel : public Model {
protected :
	MeshModel() {}
	vector<Material> mVertexMaterials;
	mat4 mModelTransform;
	mat4 mWorldTransform;
	mat4 mNormalTransform;

public:
	MeshModel(string fileName);
	~MeshModel(void);
	virtual void loadFile(string fileName);
	void initVertexNormalBuffer(vector<vec3>& vertexNormals);
	int initFlatBuffer(GLuint program);
	int initGouraudBuffer(GLuint program);
	int initShaderBuffer(RasterizerPtr rasterizer);
	void draw(RasterizerPtr rasterizer, const mat4& from3dTo2d) override;
	void drawVertexNormals(GLuint miscProgram, const mat4& finalTransform);
	void drawFaceNormals(GLuint miscProgram, const mat4& from3dTo2d);
	void setMaterialProperties() override;
	void setMaterialProperties(const Color& color) override;
	void setMaterialProperties(const Material& material) override;
	void transform(const mat4& m, const mat4& g, bool transformWorld) override;
	vector<vec3> calcVertexNormals();
	vector<GLfloat> calcFaceNormals();
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
};

class PyramidMeshModel : public MeshModel {
public:
	PyramidMeshModel();
};

typedef shared_ptr<Model> ModelPtr;
