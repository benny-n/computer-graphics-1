#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include "Renderer.h"
using namespace std;

class Model {

public:
	void virtual transform(const mat4& m, bool is_rotation = false) = 0;
	void virtual draw(Renderer&) = 0;

protected:
	virtual ~Model() {}
	class BoundryBox {
	public:
		vec4 vmin;
		vec4 vmax;

		BoundryBox() : vmin(vec3(FLT_MAX)), vmax(vec3(FLT_MIN)) {}
		vec4 center();
		void draw(Renderer* renderer);
	};
	BoundryBox boundry_box;
};


class Light {

};

class Camera {
	mat4 cTransform;
	mat4 projection;

public:
	void setTransformation(const mat4& transform);
	mat4 getTransform();
	mat4 getProjection();
	void LookAt(const vec4& eye, const vec4& at, const vec4& up);
	void Ortho( const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar );
	void Frustum( const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar );
	void Perspective( const float fovy, const float aspect,
		const float zNear, const float zFar);
};

typedef shared_ptr<Camera> CameraPtr;
typedef shared_ptr<Model> ModelPtr;
typedef shared_ptr<Light> LightPtr;

class Scene {
	vector<ModelPtr> models;
	vector<LightPtr> lights;
	vector<CameraPtr> cameras;
	Renderer *m_renderer;

public:
	Scene();
	Scene(Renderer *renderer);
	void loadOBJModel(string fileName);
	void loadCubeModel();
	void loadPyramidModel();
	int transformActiveModel(const mat4& m, bool is_rotation = false);
	void draw();
	void drawDemo();
	
	int activeModel;
	int activeLight;
	int activeCamera;
};