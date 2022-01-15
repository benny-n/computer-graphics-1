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
		vec2 mImmutableMinVec;
		vec2 mImmutableMaxVec;

		BoundryBox() : mVertexPositions(72), mMinVec(vec3(FLT_MAX)), mMaxVec(vec3(FLT_MIN)) {}
		void initVertexPositions();
		void transform(const mat4& m);
		vec4 center();
		void draw(GLuint miscProgram, GLfloat transformation[]);
	};
	string mName;
	vector<GLfloat> mVertexPositions;
	vector<GLfloat> mVertexNormals;
	vector<GLfloat> mVertexTex;
	vector<GLfloat> mVertexTB;
	vector<GLfloat> mFaceNormals;
	GLuint mVertexNormalBuffer;
	GLuint mFaceNormalBuffer;
	GLuint mTexture;
	bool mUseTexture;
	GLuint mNormalMap;
	bool mUseNormalMap;
	bool mUseWood;

	void calcTB();

public:
	bool mDrawBoundryBox;
	bool mDrawVertexNormals;
	bool mDrawFaceNormals;
	BoundryBox mBoundryBox;

	Model() : mTexture(0), mUseTexture(false), mNormalMap(0), mUseNormalMap(false), mUseWood(false),
		mDrawBoundryBox(false), mDrawVertexNormals(false), mDrawFaceNormals(false) {}
	const string& getName();
	const int getNumVertices();
	void setTexture(bool wood = false);
	void setTexture(GLuint tex);
	void setNormalMap();
	void setNormalMap(GLuint tex);
	void projectionOnPlain();
	void projectionOnCylinder();
	virtual const Material& getMaterial() = 0;
	virtual void growVertex(int vertexIndex) = 0;
	virtual void setMaterialProperties() = 0;
	virtual void setMaterialProperties(const Color& color) = 0;
	virtual void setMaterialProperties(const Material& material) = 0;
	virtual void setMaterialProperties(int index, const Material& material) = 0;
	virtual void transform(const mat4& m , const mat4& g, bool transformWorld) = 0;
	virtual void draw(RasterizerPtr rasterizer, const mat4& from3dTo2d) = 0;
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
	int initSmoothBuffer(GLuint program);
	int initNMBuffer(GLuint program);
	int initShaderBuffer(RasterizerPtr rasterizer);
	const Material& getMaterial() override;
	void growVertex(int VertexIndex) override;
	void draw(RasterizerPtr rasterizer, const mat4& from3dTo2d) override;
	void drawVertexNormals(GLuint miscProgram, const mat4& finalTransform);
	void drawFaceNormals(GLuint miscProgram, const mat4& from3dTo2d);
	void setMaterialProperties() override;
	void setMaterialProperties(const Color& color) override;
	void setMaterialProperties(const Material& material) override;
	void setMaterialProperties(int index, const Material& material) override;
	void transform(const mat4& m, const mat4& g, bool transformWorld) override;
	vector<vec3> calcVertexNormals();
	vector<GLfloat> calcFaceNormals();
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
