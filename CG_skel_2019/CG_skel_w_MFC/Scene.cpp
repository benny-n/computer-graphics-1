#include "stdafx.h"
#include "Scene.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;
GLuint handle;

const char pathSeparator =
#ifdef _WIN32
'\\';
#else
'/';
#endif

// Scene

Scene::Scene() : mRenderCameras(false), mControlledElement(SceneElement::Camera), mControlWorld(false),
				 mActiveModel(0), mActiveLight(0), mActiveCamera(0), mSkyboxTex(0), mUseSkybox(false) {
	initOpenGLRendering();
	createBuffers(512, 512);
	auto defaultCamera = make_shared<Camera>();
	mCameras.push_back(defaultCamera);
	auto ambientLight = make_shared<AmbientLight>();
	mLights.push_back(ambientLight);
	mRasterizer = make_shared<Rasterizer>();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);

	mSkyboxBuffer = vector<GLfloat>{
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
}

const vector<ModelPtr>& Scene::getModels() { return mModels; }

const vector<CameraPtr>& Scene::getCameras() { return mCameras; }

const vector<LightPtr>& Scene::getLights() { return mLights; }

const SceneElement& Scene::getControlledElement() { return mControlledElement; }

const Material& Scene::getActiveModelMaterial() { return mModels[mActiveModel]->getMaterial(); }

const int Scene::getActiveModelNumVertices() { return mModels[mActiveModel]->getNumVertices(); }

void Scene::setRasterizer(ShaderType shaderType) { mRasterizer->setActiveProgram(shaderType); }

void Scene::loadOBJModel(string fileName) {
	auto model = make_shared<MeshModel>(fileName);
	mModels.push_back(model);
	mActiveModel = mModels.size() - 1;
	mControlledElement = SceneElement::Model;
	printControlMsg();
}

void Scene::loadTexture(string fileName) {
    GLint width, height, channel_count;
    stbi_set_flip_vertically_on_load(true);
    
    GLubyte *data = stbi_load(fileName.c_str(), &width, &height, &channel_count, 0);
    if(!data) return;
    
	GLuint handle;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);

	mModels[mActiveModel]->setTexture(handle);
}

void Scene::loadNormalMap(string fileName) {
	TCHAR NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);
	GLint width, height, channel_count;
	stbi_set_flip_vertically_on_load(true);

	GLubyte* data = stbi_load(fileName.c_str(), &width, &height, &channel_count, 0);
	if (!data) return;

	GLuint handle;
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	mModels[mActiveModel]->setNormalMap(handle);
}

void Scene::loadSkybox(string title) {
	if (mSkyboxTex != 0) {
		glDeleteTextures(1, &mSkyboxTex);
		mSkyboxTex = 0;
	}
	vector<std::string> faces = { 
		"skyboxes\\" + title + "\\right.jpg",
		"skyboxes\\" + title + "\\left.jpg",
		"skyboxes\\" + title + "\\top.jpg",
		"skyboxes\\" + title + "\\bottom.jpg",
		"skyboxes\\" + title + "\\front.jpg",
		"skyboxes\\" + title + "\\back.jpg" };

	glGenTextures(1, &mSkyboxTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mSkyboxTex);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	mUseSkybox = true;
}

void Scene::removeSkybox() { 
	if (mSkyboxTex != 0) {
		glDeleteTextures(1, &mSkyboxTex);
		mSkyboxTex = 0;
	}
	for each (auto model in mModels) {
		model->setReflectivity(0);
	}
	mUseSkybox = false; 
}

bool Scene::skyboxActive() {
	return mUseSkybox;
}

void Scene::addCubeModel() {
	auto model = make_shared<CubeMeshModel>();
	mModels.push_back(model);
	mActiveModel = mModels.size() - 1;
	mControlledElement = SceneElement::Model;
	printControlMsg();
}

void Scene::addPyramidModel() {
	auto model = make_shared<PyramidMeshModel>();
	mModels.push_back(model);
	mActiveModel = mModels.size() - 1;
	mControlledElement = SceneElement::Model;
	printControlMsg();
}

void Scene::addCamera(){
	auto camera = make_shared<Camera>();
	mCameras.push_back(camera);
	mActiveCamera = mCameras.size() - 1;
	mControlledElement = SceneElement::Camera;
	printControlMsg();
}

void Scene::addPointLight(const vec3& p) {
	auto Light = make_shared<PointLight>(p);
	mLights.push_back(Light);
	mActiveLight = mLights.size() - 1;
	mControlledElement = SceneElement::Light;
	printControlMsg();
}

void Scene::addParallelLight(const vec3& n) {
	auto Light = make_shared<ParallelLight>(n);
	mLights.push_back(Light);
	mActiveLight = mLights.size() - 1;
	mControlledElement = SceneElement::Light;
	printControlMsg();
}

void Scene::toggleControlWorld() {
	mControlWorld = !mControlWorld;
	printControlMsg();
}

void Scene::initOpenGLRendering()
{
	int a = glGetError();
	a = glGetError();
	glGenTextures(1, &mScreenTex);
	a = glGetError();
	glGenVertexArrays(1, &mScreenVtc);
	GLuint buffer;
	glBindVertexArray(mScreenVtc);
	glGenBuffers(1, &buffer);
	const GLfloat vtc[] = {
		-1, -1,
		1, -1,
		-1, 1,
		-1, 1,
		1, -1,
		1, 1
	};
	const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);
	GLint  vPosition = glGetAttribLocation(program, "vPosition");

	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0,
		0);

	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		(GLvoid*)sizeof(vtc));
	glProgramUniform1i(program, glGetUniformLocation(program, "texture"), 0);
	a = glGetError();
}

void Scene::createBuffers(int width, int height) {
	mWidth = width;
	mHeight = height;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mScreenTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, mWidth, mHeight);
	glutReshapeWindow(width, height);
}

void Scene::toggleRenderCameras(){
	mRenderCameras = !mRenderCameras;
}

void Scene::togglePlotBoundryBox(){
	mModels[mActiveModel]->mDrawBoundryBox = !mModels[mActiveModel]->mDrawBoundryBox;
}

void Scene::togglePlotVertexNormals(){
	mModels[mActiveModel]->mDrawVertexNormals = !mModels[mActiveModel]->mDrawVertexNormals;
}

void Scene::togglePlotFaceNormals(){
	mModels[mActiveModel]->mDrawFaceNormals = !mModels[mActiveModel]->mDrawFaceNormals;
}

void Scene::changeActiveModelMaterial() {
	mModels[mActiveModel]->setMaterialProperties();
}

void Scene::changeActiveModelMaterial(const Color& color) {
	mModels[mActiveModel]->setMaterialProperties(color);
}

void Scene::changeActiveModelMaterial(const Material& material) {
	mModels[mActiveModel]->setMaterialProperties(material);
}

void Scene::changeActiveModelVertexMaterial(int index, const Material& nextMaterial) {
	mModels[mActiveModel]->setMaterialProperties(index, nextMaterial);
}

void Scene::changeActiveLightColor(const Color& color) { mLights[mActiveLight]->setColor(color); }

void Scene::changeActiveLightColor(const Color& c1, const Color& c2, const Color& c3) { 
	mLights[mActiveLight]->setColor(c1, c2, c3); 
}

void Scene::growActiveModelVertex(int vertexIndex) { mModels[mActiveModel]->growVertex(vertexIndex); }

//SCALING
void Scene::transformActive(const mat4& m){ 
	switch (mControlledElement) {
	case SceneElement::Camera:
		mCameras[mActiveCamera]->transform(m);
		break;
	case SceneElement::Light:
		mLights[mActiveLight]->modifyIntensities(vec3(m[0][0], m[1][1], m[2][2]));
		break;
	case SceneElement::Model:
		mat4 g = scale(1 / m[0][0], 1 / m[1][1], 1 / m[2][2]);
		mat4 transformation = translate(mModels[mActiveModel]->mBoundryBox.center()) * m * translate(-mModels[mActiveModel]->mBoundryBox.center());
		mModels[mActiveModel]->transform(transformation, g, mControlWorld);
		break;
	}
}

//ROTATION
void Scene::transformActive(const float degrees, const RotationAxis& axis) { 
	switch (mControlledElement) {
	case SceneElement::Camera:
		mCameras[mActiveCamera]->transform(degrees, axis);
		break;
	case SceneElement::Light: {
		mat4 rotation;
		switch (axis) {
		case RotationAxis::X:
			rotation = rotateX(degrees);
			break;
		case RotationAxis::Y:
			rotation = rotateY(degrees);
			break;
		case RotationAxis::Z:
			rotation = rotateZ(degrees);
			break;
		}
		mLights[mActiveLight]->rotate(rotation);
		break;
	}
	case SceneElement::Model: {
		mat4 rotation;
		switch (axis) {
		case RotationAxis::X:
			rotation = rotateX(-degrees);
			break;
		case RotationAxis::Y:
			rotation = rotateY(-degrees);
			break;
		case RotationAxis::Z:
			rotation = rotateZ(degrees);
			break;
		}
		mModels[mActiveModel]->transform(rotation, rotation, mControlWorld);
		break;
	}
	}
}

//TRANSLATE
void Scene::transformActive(const vec3& v) {
	switch (mControlledElement) {
	case SceneElement::Camera:
		mCameras[mActiveCamera]->transform(v);
		break;
	case SceneElement::Light:
		mLights[mActiveLight]->translate(v);
		break;
	case SceneElement::Model:
		mModels[mActiveModel]->transform(translate(v), mat4(), mControlWorld);
		break;
	}
}

inline static void getNewUp(vec4& newUp, const vec3& eye, const vec3& at) {
	if (at == vec3()) { // use eye
		if (eye.x == 0) newUp = vec3(1, 0, 0);
		else if (eye.y == 0) newUp = vec3(0, 1, 0);
		else if (eye.z == 0) newUp = vec3(0, 0, 1);
		else newUp = vec3(0, 1, -eye.y / eye.z);
	}
	else {
		if (at.x == 0) newUp = vec3(1, 0, 0);
		else if (at.y == 0) newUp = vec3(0, 1, 0);
		else if (at.z == 0) newUp = vec3(0, 0, 1);
		else newUp = vec3(0, 1, -at.y / at.z);
	}
}

void Scene::modifyActiveCamera(const vec4& v, bool isEye) {
	const CameraPtr activeCamera = mCameras[mActiveCamera];
 	vec3 vector0, modifiedEye, modifiedAt;
	if (isEye) {
		const vec4 at = activeCamera->getAt();
		modifiedAt = vec3FromVec4(at);
		modifiedEye = vec3FromVec4(v);
		
		vec4 newUp = cross(modifiedEye, modifiedAt);
		if (newUp == vector0)
			getNewUp(newUp, modifiedEye, modifiedAt);
		newUp = normalize(newUp);
		activeCamera->lookAt(v, at, newUp);
	}
	else { // is at
		const vec4 eye = activeCamera->getEye();
		modifiedAt = vec3FromVec4(v);
		modifiedEye = vec3FromVec4(eye);
		vec4 newUp = cross(modifiedEye, modifiedAt);
		if (newUp == vector0)
			getNewUp(newUp, modifiedEye, modifiedAt);
		newUp = normalize(newUp);
		activeCamera->lookAt(eye, v, newUp);
	}
}


void Scene::modifyActiveLight(const vec3& v, bool isPosition) {
	if (isPosition) {
		if (mLights[mActiveLight]->getType() != LightType::Point) return; // should never happen
		shared_ptr<PointLight> pointLight = dynamic_pointer_cast<PointLight>(mLights[mActiveLight]);
		pointLight->setPosition(v);
	}
	else {
		if (mLights[mActiveLight]->getType() != LightType::Parallel) return; // should never happen
		shared_ptr<ParallelLight> parallelLight = dynamic_pointer_cast<ParallelLight>(mLights[mActiveLight]);
		parallelLight->setDirection(v);
	}
}


void Scene::reshape(int width, int height) {
	createBuffers(width, height);
	if (mWidth > mHeight) mAspectRatioTransform = scale(1, mWidth / (float)mHeight, 1);
	else  mAspectRatioTransform = scale(mHeight / (float)mWidth, 1, 1);

}

void Scene::iterateControlledElement()
{
	switch (mControlledElement) {
	case SceneElement::Camera:
		mControlledElement = SceneElement::Light;
		break;
	case SceneElement::Light:
		if (mModels.size() > 0) mControlledElement = SceneElement::Model;
		else mControlledElement = SceneElement::Camera;
		break;
	case SceneElement::Model:
		mControlledElement = SceneElement::Camera;
		break;
	}
	printControlMsg();
}

void Scene::iterateActive() {
	switch (mControlledElement) {
	case SceneElement::Camera:
		mActiveCamera = (mActiveCamera + 1) % mCameras.size();
		break;
	case SceneElement::Light:
		mActiveLight = (mActiveLight + 1) % mLights.size();
		initMenu();
		break;
	case SceneElement::Model:
		if (!mModels.empty()) mActiveModel = (mActiveModel + 1) % mModels.size();
		break;
	}
	printControlMsg();
}

void Scene::printControlMsg() {
	cout << "Controlling ";
	switch (mControlledElement) {
	case SceneElement::Camera:
		cout << "camera " << mActiveCamera;
		break;
	case SceneElement::Light:
		cout << mLights[mActiveLight]->getTypeString() << " light";
		if (mLights[mActiveLight]->getType() != LightType::Ambient)
			cout << " " << mActiveLight;
		break;
	case SceneElement::Model:
		cout << mModels[mActiveModel]->getName() << " " << mActiveModel;
		if (mControlWorld) cout << " (world)";
		break;
	}
	cout << endl;
}

void Scene::removeActiveModel()
{
	vector<ModelPtr>::iterator iter = mModels.begin() + mActiveModel;
	mModels.erase(iter);
	if (mActiveModel > 0) mActiveModel--;
	else if (mModels.size() > 0) mActiveModel = mModels.size() - 1;
	else mControlledElement = SceneElement::Camera;
	if (mControlledElement == SceneElement::Model) printControlMsg();
}

void Scene::removeActiveCamera()
{
	if (mCameras.size() == 1) return; // should never happen
	vector<CameraPtr>::iterator iter = mCameras.begin() + mActiveCamera;
	mCameras.erase(iter);
	if (mActiveCamera > 0) mActiveCamera--;
	else mActiveCamera = mCameras.size() - 1;
	if (mControlledElement == SceneElement::Camera) printControlMsg();
}

void Scene::removeActiveLight()
{
	if (mActiveLight == 0) return; // should never happen
	vector<LightPtr>::iterator iter = mLights.begin() + mActiveLight;
	mLights.erase(iter);
	if (mActiveLight > 0) mActiveLight--;
	else mActiveLight = mLights.size() - 1;
	if (mControlledElement == SceneElement::Light) printControlMsg();
}

void Scene::setGlLights(GLuint program) {
	glUseProgram(program);
	for (int i = 0; i < mLights.size(); i++) {
		std::string currentLightStr = std::string("lights[" + std::to_string(i) + "]");
		std::string tempString = currentLightStr + ".type";
		GLuint currentLightType = glGetUniformLocation(program, tempString.c_str());
		tempString = currentLightStr + ".position";
		GLuint currentLightPosition = glGetUniformLocation(program, tempString.c_str());
		tempString = currentLightStr + ".direction";
		GLuint currentLightDirection = glGetUniformLocation(program, tempString.c_str());
		tempString = currentLightStr + ".la";
		GLuint currentLightLa = glGetUniformLocation(program, tempString.c_str());
		tempString = currentLightStr + ".ld";
		GLuint currentLightLd = glGetUniformLocation(program, tempString.c_str());
		tempString = currentLightStr + ".ls";
		GLuint currentLightLs = glGetUniformLocation(program, tempString.c_str());

		glUniform1i(currentLightType, (GLint)mLights[i]->getType());
		glUniform3fv(currentLightPosition, 1, mLights[i]->getPosition());
		glUniform3fv(currentLightDirection, 1, mLights[i]->getDirection());
		glUniform3fv(currentLightLa, 1, mLights[i]->mLa.toVec());
		glUniform3fv(currentLightLd, 1, mLights[i]->mLd.toVec());
		glUniform3fv(currentLightLs, 1, mLights[i]->mLs.toVec());
	}

	GLuint numLights = glGetUniformLocation(program, "numLights");
	glUniform1i(numLights, (GLsizei)mLights.size());
	GLuint eyeLoc = glGetUniformLocation(program, "eye");
	glUniform3fv(eyeLoc, 1, mCameras[mActiveCamera]->getEye());
}

void Scene::drawSkybox() {
	// switch off depth so box is in background
	glDepthMask(GL_FALSE);
	glEnable(GL_DEPTH_CLAMP);
	GLuint program = mRasterizer->getSkyboxProgram();
	glUseProgram(program);
	auto activeCamera = mCameras[mActiveCamera];
	const mat4 projection = activeCamera->getProjection();
	const mat4 rotations = activeCamera->getRotations();
	const mat4 from3dTo2d = projection * rotations;
	// send transform
	GLuint transformLoc = glGetUniformLocation(program, "transform");
	GLfloat transform[16];
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			transform[i * 4 + j] = from3dTo2d[i][j];
		}
	}
	glUniformMatrix4fv(transformLoc, 1, GL_TRUE, transform);
	// fit the skybox to the viewing window size (1.5 to ensure camera doesn't slip out of the skybox)
	float maxSide = 1.5 * activeCamera->getMaxSideLength();
	vector<GLfloat> skyboxFittedBuffer(mSkyboxBuffer.size());
	for (int i = 0; i < mSkyboxBuffer.size(); i++)
		skyboxFittedBuffer[i] = maxSide * mSkyboxBuffer[i];
	// bind the box
	GLuint skyboxBuffer;
	glGenBuffers(1, &skyboxBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * skyboxFittedBuffer.size(), skyboxFittedBuffer.data(), GL_STATIC_DRAW);
	GLuint loc = glGetAttribLocation(program, "vPosition");
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// draw
	glBindTexture(GL_TEXTURE_CUBE_MAP, mSkyboxTex);
	glDrawArrays(GL_TRIANGLES, 0, skyboxFittedBuffer.size());
	// return gl to correct state for models
	glDepthMask(GL_TRUE);
	glDisable(GL_DEPTH_CLAMP);
	// bind the box texture to other programs
	glUseProgram(mRasterizer->getActiveProgram());
	glBindTexture(GL_TEXTURE_CUBE_MAP, mSkyboxTex);
	glUseProgram(mRasterizer->getNMProgram());
	glBindTexture(GL_TEXTURE_CUBE_MAP, mSkyboxTex);
}

void Scene::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setGlLights(mRasterizer->getActiveProgram());
	setGlLights(mRasterizer->getNMProgram());

	auto activeCamera = mCameras[mActiveCamera];
	const mat4 projection = activeCamera->getProjection();
	const mat4 cameraTransform = activeCamera->getTransform();
	const mat4 from3dTo2d = projection * cameraTransform * mAspectRatioTransform;

	if (mUseSkybox) drawSkybox();

	for each (auto model in mModels) {
		model->draw(mRasterizer, from3dTo2d);
	}

	if (mRenderCameras) {
		for each (auto camera in mCameras) {
			camera->draw(mRasterizer, from3dTo2d);
		}
	}

	glFlush();
	glutSwapBuffers();
}


