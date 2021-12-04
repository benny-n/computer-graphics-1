#include "stdafx.h"
#include "Controls.h"
#include<limits>

using namespace std;

int gLastX, gLastY;
bool gLbDown, gRbDown, gMbDown;
float gSensetivity = 1;
extern Scene* gScene;
extern Renderer* gRenderer;


float getFloatFromUser(const string& directive, bool wantFraction)
{
	float input = -1;
	cout << directive << ": ";
	cin >> input;
	while (!cin || (wantFraction && (input < 0 || input > 1))) {
		// Clear the error flag
		cin.clear();
		// Ignore the characters to avoid looping forever
		cin.ignore(LLONG_MAX, '\n');
		AfxMessageBox(_T("The value you entered is invalid.\n"
						  "Please try again"));
		cout << directive << ": ";
		cin >> input;
	}
	return input;
}

void inputMessage() { AfxMessageBox(_T("Please enter your wanted parameters in the console window")); }

//----------------------------------------------------------------------------
// Callbacks
void display(void) { gScene->draw(); }

void reshape(int width, int height)
{
	//cout << width << " " << height << endl;
	gRenderer->reshape(width, height);
	gScene->draw();
}

void keyboard(unsigned char key, int x, int y)
{
	//cout << "pressed key: " << key << " key's number: " << int(key) << endl;
	switch (key) {
	case 033: // escape
		exit(EXIT_SUCCESS);
		break;	
	case 'z':
		gScene->transformActive(scale(SCALE_UP));
		break;
	case 'x':
		gScene->transformActive(scale(SCALE_DOWN));
		break;
	case 'D':
		 gScene->transformActive(ROTATE, RotationAxis::Y);
		break;
	case 'A':
		 gScene->transformActive(-ROTATE, RotationAxis::Y);
		break;	
	case 'W':
		 gScene->transformActive(ROTATE, RotationAxis::X);
		break;
	case 'S':
		 gScene->transformActive(-ROTATE, RotationAxis::X);
		break;
	case 'Q':
		 gScene->transformActive(ROTATE, RotationAxis::Z);
		break;
	case 'E':
		 gScene->transformActive(-ROTATE, RotationAxis::Z);
		break;
	case 'd':
		 gScene->transformActive(vec3(TRANSLATE, 0, 0));
		break;	
	case 'a':
		 gScene->transformActive(vec3(-TRANSLATE, 0, 0));
		break;
	case 'w':
		 gScene->transformActive(vec3(0, TRANSLATE, 0));
		break;
	case 's':
		 gScene->transformActive(vec3(0, -TRANSLATE, 0));
		break;
	case 'q':
		 gScene->transformActive(vec3(0, 0, TRANSLATE));
		break;
	case 'e':
		 gScene->transformActive(vec3(0, 0, -TRANSLATE));
		break;
	case 'c':
		gScene->toggleRenderCameras();
		break;
	case 'j':
		if (gScene->getControlledElement() == SceneElement::Model)
			gScene->transformActive(scale(SCALE_UP, 1, 1));
		break;
	case 10: // ctrl + j
		if (gScene->getControlledElement() == SceneElement::Model)
			gScene->transformActive(scale(SCALE_DOWN, 1, 1));
		break;
	case 'k':
		if (gScene->getControlledElement() == SceneElement::Model)
			gScene->transformActive(scale(1, SCALE_UP, 1));
		break;
	case 11: // ctrl + k
		if (gScene->getControlledElement() == SceneElement::Model)
			gScene->transformActive(scale(1, SCALE_DOWN, 1));
		break;
	case 'l':
		if (gScene->getControlledElement() == SceneElement::Model)
			gScene->transformActive(scale(1, 1, SCALE_UP));
		break;
	case 12: // ctrl + l
		if (gScene->getControlledElement() == SceneElement::Model)
			gScene->transformActive(scale(1, 1, SCALE_DOWN));
		break;
	case '\t': // tab
		gScene->iterateActive();
		break;
	case ' ': // space bar
		gScene->iterateControlledElement();
		break;
	case 23: // ctrl + w
		gScene->toggleControlWorld();
		break;

	default:
		return;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	//button = {GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON}
	//state = {GLUT_DOWN,GLUT_UP}
	int status = 0;
	//set down flags
	switch (button) {
	case GLUT_LEFT_BUTTON:
		//cout << x << " , " << y << endl;
		gLbDown = (state == GLUT_UP) ? 0 : 1;
		break;
	case GLUT_RIGHT_BUTTON:
		gRbDown = (state == GLUT_UP) ? 0 : 1;
		break;
	case GLUT_MIDDLE_BUTTON:
		gMbDown = (state == GLUT_UP) ? 0 : 1;
		break;
	case MOUSE_WHEEL_UP:
		gScene->transformActive(scale(SCALE_UP));
		break;
	case MOUSE_WHEEL_DOWN:
		gScene->transformActive(scale(SCALE_DOWN));
		break;
	}
	glutPostRedisplay();
}

void special(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP:
		gSensetivity += 0.1;
		break;
	case GLUT_KEY_DOWN:
		if (gSensetivity < 0.2) AfxMessageBox(_T("Sensetivity at minimum!"));
		else gSensetivity -= 0.1;
		break;
	}
}

void motion(int x, int y)
{
	// calc difference in mouse movement
	int dx = x - gLastX;
	int dy = y - gLastY;
	// update last x,y
	gLastX = x;
	gLastY = y;
}

void fileMenu(int id)
{
	CFileDialog dlg(TRUE, _T(".obj"), NULL, NULL, _T("*.obj|*.*"));
	if (dlg.DoModal() == IDOK)
	{
		gScene->loadOBJModel((LPCTSTR)dlg.GetPathName());
		glutPostRedisplay();
		initMenu();
	}
}

void addPrimMenu(int id) {
	switch (id)
	{
	case CUBE:
		gScene->addCubeModel();
		break;
	case PYRAMID:
		gScene->addPyramidModel();	
		break;
	}
	glutPostRedisplay();
	initMenu();
}

void selectModelMenu(int id) {
	gScene->mActiveModel = id;
}

void changeMaterialMenu(int id) {
	switch (id)
	{
	case UNIFORM_MATERIAL: {
		inputMessage();
		cout << "Please enter material property values" << endl;
		float ka = getFloatFromUser("ambient", true);
		float kd = getFloatFromUser("diffuse", true);
		float ks = getFloatFromUser("specular", true);
		float alpha = getFloatFromUser("shininess", false);
		gScene->changeActiveModelMaterial(vec4(ka, kd, ks, alpha));
		break;
	}
	case NON_UNIFORM_MATERIAL:
		gScene->changeActiveModelMaterial();
		break;
	}
	glutPostRedisplay();
}

void changeModelColorMenu(int id) {
	switch (id)
	{
	case WHITE:
		gScene->changeActiveModelColor(vec3(1));
		break;
	case RED:
		gScene->changeActiveModelColor(vec3(1, 0, 0));
		break;
	case GREEN:
		gScene->changeActiveModelColor(vec3(0, 1, 0));
		break;
	case BLUE:
		gScene->changeActiveModelColor(vec3(0, 0, 1));
		break;
	case YELLOW:
		gScene->changeActiveModelColor(vec3(1, 1, 0));
		break;
	case CUSTOM_COLOR:
		inputMessage();
		cout << "Please enter rgb values between 0 and 1" << endl;
		float r = getFloatFromUser("r", true);
		float g = getFloatFromUser("g", true);
		float b = getFloatFromUser("b", true);
		//cout << "Custom color: (" << r << ", " << g << ", " << b << ")" << endl;
		gScene->changeActiveModelColor(vec3(r, g, b));
		break;
	}
	glutPostRedisplay();
}

void activeModelOptionsMenu(int id) {
	switch (id)
	{
	case PLOT_BOUNDRY_BOX:
		gScene->togglePlotBoundryBox();
		break;
	case PLOT_VERTEX_NORMALS:
		gScene->togglePlotVertexNormals();
		break;
	case PLOT_FACE_NORMALS:
		gScene->togglePlotFaceNormals();
		break;
	case MOVE_MODEL_TO: {
		inputMessage();
		cout << "Please enter coordinates for the model's new location" << endl;
		float x = getFloatFromUser("x");
		float y = getFloatFromUser("y");
		float z = getFloatFromUser("z");
		//cout << "New coordinates: (" << x << ", " << y << ", " << z << ")" << endl;
		const vec4 modelCenter = gScene->getModels()[gScene->mActiveModel]->mBoundryBox.center();
		gScene->transformActive(translate(x - modelCenter.x, y - modelCenter.y, z - modelCenter.z));
		break;
	}
	case REMOVE_ACTIVE_MODEL:
		gScene->removeActiveModel();
		initMenu();
		break;
	}
	glutPostRedisplay();
}

void selectCameraMenu(int id) {
	gScene->mActiveCamera = id;
	gScene->printControlMsg();
}

void activeCameraOptionsMenu(int id) {
	float left, right, bottom, top, zNear, zFar, fovy, aspect, x, y, z;
	switch (id)
	{
	case FOCUS: {
		const vec4 activeModelCenter = gScene->getModels()[gScene->mActiveModel]->mBoundryBox.center();
		gScene->modifyActiveCamera(activeModelCenter, false);
		break;
	}
	case ORTHO:
		inputMessage();
		cout << "Please enter your desired parameters" << endl;
		left = getFloatFromUser("left");
		right = getFloatFromUser("right");
		bottom = getFloatFromUser("bottom");
		top = getFloatFromUser("top");
		zNear = getFloatFromUser("zNear");
		zFar = getFloatFromUser("zFar");
		gScene->getCameras()[gScene->mActiveCamera]->ortho(left, right, bottom, top, zNear, zFar);
		break;
	case FRUSTUM:
		inputMessage();
		cout << "Please enter your desired parameters" << endl;
		left = getFloatFromUser("left");
		right = getFloatFromUser("right");
		bottom = getFloatFromUser("bottom");
		top = getFloatFromUser("top");
		zNear = getFloatFromUser("zNear");
		zFar = getFloatFromUser("zFar");
		gScene->getCameras()[gScene->mActiveCamera]->frustum(left, right, bottom, top, zNear, zFar);
		break;
	case PERSPECTIVE:
		inputMessage();
		cout << "Please enter your desired parameters" << endl;
		fovy = getFloatFromUser("fovy");
		aspect = getFloatFromUser("aspect ratio");
		zNear = getFloatFromUser("zNear");
		zFar = getFloatFromUser("zFar");
		gScene->getCameras()[gScene->mActiveCamera]->perspective(fovy, aspect, zNear, zFar);
		break;
	case LOOK_AT:
		inputMessage();
		cout << "Please enter coordinates for the camera's new target" << endl;
		x = getFloatFromUser("x");
		y = getFloatFromUser("y");
		z = getFloatFromUser("z");
		gScene->modifyActiveCamera(vec4(x, y, z, 1), false);
		break;
	case MOVE_CAMERA_TO:
		inputMessage();
		cout << "Please enter coordinates for the camera's new location" << endl;
		x = getFloatFromUser("x");
		y = getFloatFromUser("y");
		z = getFloatFromUser("z");
		gScene->modifyActiveCamera(vec4(x, y, z, 1), true);
		break;
	case REMOVE_ACTIVE_CAMERA:
		if (gScene->getCameras().size() == 1) AfxMessageBox(_T("You can't remove your only camera!"));
		else {
			gScene->removeActiveCamera();
			initMenu();
		}
		break;
	}
	glutPostRedisplay();
}

void addLightMenu(int id) {
	float x, y, z;
	switch (id) {
	case POINT:
		inputMessage();
		cout << "Please enter coordinates for the light's location" << endl;
		x = getFloatFromUser("x");
		y = getFloatFromUser("y");
		z = getFloatFromUser("z");
		gScene->addPointLight(vec3(x, y, z));
		break;
	case PARALLEL:
		inputMessage();
		cout << "Please enter values for the light's direction" << endl;
		x = getFloatFromUser("x");
		y = getFloatFromUser("y");
		z = getFloatFromUser("z");
		gScene->addParallelLight(vec3(x, y, z));
		break;
	}
	glutPostRedisplay();
	initMenu();
}

void selectLightMenu(int id) {
	gScene->mActiveLight = id;
	gScene->printControlMsg();
}

void changeLightColorMenu(int id) {
	switch (id) {
	case WHITE:
		gScene->changeActiveLightColor(vec3(1));
		break;
	case RED:
		gScene->changeActiveLightColor(vec3(1, 0, 0));
		break;
	case GREEN:
		gScene->changeActiveLightColor(vec3(0, 1, 0));
		break;
	case BLUE:
		gScene->changeActiveLightColor(vec3(0, 0, 1));
		break;
	case YELLOW:
		gScene->changeActiveLightColor(vec3(1, 1, 0));
		break;
	case CUSTOM_COLOR:
		inputMessage();
		cout << "Please enter rgb values between 0 and 1" << endl;
		float r = getFloatFromUser("r", true);
		float g = getFloatFromUser("g", true);
		float b = getFloatFromUser("b", true);
		//cout << "Custom color: (" << r << ", " << g << ", " << b << ")" << endl;
		gScene->changeActiveLightColor(vec3(r, g, b));
		break;
	}
	glutPostRedisplay();
}

void activeLightOptionsMenu(int id) {
	float x, y, z;
	switch (id) {
	case MOVE_LIGHT_TO: {
		inputMessage();
		cout << "Please enter coordinates for the light's new location" << endl;
		x = getFloatFromUser("x");
		y = getFloatFromUser("y");
		z = getFloatFromUser("z");
		gScene->modifyActiveLight(vec3(x, y, z), true);
		break;
	}
	case CHANGE_DIRECTION_TO:
		inputMessage();
		cout << "Please enter values for light's new direction" << endl;
		x = getFloatFromUser("x");
		y = getFloatFromUser("y");
		z = getFloatFromUser("z");
		gScene->modifyActiveLight(vec3(x, y, z), false);
		break;
	case REMOVE_ACTIVE_LIGHT:
		if (gScene->mActiveLight == 0) AfxMessageBox(_T("You can't remove the ambient light!"));
		else {
			gScene->removeActiveLight();
			initMenu();
		}
		break;
	}
}

void mainMenu(int id) {
	switch (id)
	{
	case ADD_CAMERA:
		gScene->addCamera();
		glutPostRedisplay();
		initMenu();
		break;
	//case MAIN_DEMO:
	//	gScene->drawDemo();
	//	break;
	case HELP:
		AfxMessageBox(_T(
		"User input and messages will appear on the console window\n\n"
		"Keyboard Controls:\n"
		"a, d - moving model/camera in parralel to X axis\n"
		"s, w - moving model/camera in parralel to Y axis\n"
		"q, e - moving model/camera in parralel to Z axis\n"
		"A, D - rotating model/camera around it's Y axes\n"
		"S, W - rotating model/camera around it's X axes\n"
		"Q, E - rotating model/camera around it's Z axes\n"
		"z, x - scaling model up, down / zoomig camera in, out\n"
		"j, k, l - non-uniform scaling up along x, y, z axis\n"
		"ctrl + j, k, l - non-uniform scaling down along x, y, z axis\n\n"
		"c - switch showing cameras on/off\n"
		"tab - switch control to next model/camera\n"
		"space - switch control between model and camera\n"
		"ctrl + w - switch control between model and model's world\n\n"
		"use up and down arrows to control sensitivity\n"
		));
		break;
	}
}

void initMenu()
{

	// Must prepare all submenus before calling "creatMenu" on the main menu!
	
	//create file menu (not us)
	int menuFile = glutCreateMenu(fileMenu);
	glutAddMenuEntry("Open..", FILE_OPEN);

	//create add primitive model menu
	int menuAddPrim = glutCreateMenu(addPrimMenu);
	glutAddMenuEntry("Cube", CUBE);
	glutAddMenuEntry("Pyramid", PYRAMID);

	//create add model menu
	int menuAddModel = glutCreateMenu(nullptr); // has only sub menus so needs no function
	glutAddSubMenu("From File", menuFile);
	glutAddSubMenu("Add Primitive Model", menuAddPrim);

	//create select model menu
	int menuSelectModel = glutCreateMenu(selectModelMenu);
	int counter = 0;
	for each (auto model in gScene->getModels()) {
		glutAddMenuEntry((model->getName() + " " + to_string(counter)).c_str(), counter);
		counter++;
	}

	//create change material menu
	int menuChangeMaterial = glutCreateMenu(changeMaterialMenu);
	glutAddMenuEntry("Uniform Material", UNIFORM_MATERIAL);
	glutAddMenuEntry("Non-Uniform Material", NON_UNIFORM_MATERIAL);

	//create change model color menu
	int menuChangeModelColor = glutCreateMenu(changeModelColorMenu);
	glutAddMenuEntry("White", WHITE);
	glutAddMenuEntry("Red", RED);
	glutAddMenuEntry("Green", GREEN);
	glutAddMenuEntry("Blue", BLUE);
	glutAddMenuEntry("Yellow", YELLOW);
	glutAddMenuEntry("Choose custom color", CUSTOM_COLOR);

	//create active model options menu
	int menuActiveModelOptions = glutCreateMenu(activeModelOptionsMenu);
	glutAddMenuEntry("Plot Boundry Box", PLOT_BOUNDRY_BOX);
	glutAddMenuEntry("Plot Vertex Normals", PLOT_VERTEX_NORMALS);
	glutAddMenuEntry("Plot Face Normals", PLOT_FACE_NORMALS);
	glutAddMenuEntry("Move To", MOVE_MODEL_TO);
	glutAddSubMenu("Change Material", menuChangeMaterial);
	glutAddSubMenu("Change Color", menuChangeModelColor);
	glutAddMenuEntry("Remove Active Model", REMOVE_ACTIVE_MODEL);

	//create select camera menu
	int menuSelectCamera = glutCreateMenu(selectCameraMenu);
	counter = 0;
	for each (auto camera in gScene->getCameras()) {
		glutAddMenuEntry(("camera " + to_string(counter)).c_str(), counter);
		counter++;
	}

	//create active camera options menu
	int menuActiveCameraOptions = glutCreateMenu(activeCameraOptionsMenu);
	glutAddMenuEntry("Focus On Active Model", PLOT_BOUNDRY_BOX);
	glutAddMenuEntry("Ortho", ORTHO);
	glutAddMenuEntry("Frustum", FRUSTUM);
	glutAddMenuEntry("Perspective", PERSPECTIVE);
	glutAddMenuEntry("Look At", LOOK_AT);
	glutAddMenuEntry("Move To", MOVE_CAMERA_TO);
	glutAddMenuEntry("Remove Active Camera", REMOVE_ACTIVE_CAMERA);

	//create add light menu
	int menuAddLight = glutCreateMenu(addLightMenu);
	glutAddMenuEntry("Point", POINT);
	glutAddMenuEntry("Parallel", PARALLEL);

	//create select light menu
	int menuSelectLight = glutCreateMenu(selectLightMenu);
	counter = 0;
	for each (auto light in gScene->getLights()) {
		string index = counter == 0 ? "" : to_string(counter);
		glutAddMenuEntry((light->getTypeString() + " light " + index).c_str(), counter);
		counter++;
	}

	//create change light color menu
	int menuChangeLightColor = glutCreateMenu(changeLightColorMenu);
	glutAddMenuEntry("White", WHITE);
	glutAddMenuEntry("Red", RED);
	glutAddMenuEntry("Green", GREEN);
	glutAddMenuEntry("Blue", BLUE);
	glutAddMenuEntry("Yellow", YELLOW);
	glutAddMenuEntry("Choose custom color", CUSTOM_COLOR);

	//create active light options menu
	int menuActiveLightOptions = glutCreateMenu(activeLightOptionsMenu);
	if (gScene->getLights()[gScene->mActiveLight]->getType() == LightType::Point)
		glutAddMenuEntry("Move To", MOVE_LIGHT_TO);
	else if (gScene->getLights()[gScene->mActiveLight]->getType() == LightType::Parallel)
		glutAddMenuEntry("Change Direction", CHANGE_DIRECTION_TO);
	glutAddSubMenu("Change Color", menuChangeLightColor);
	glutAddMenuEntry("Remove Active Light", REMOVE_ACTIVE_LIGHT);

	//finally, create the main menu and start adding submenus to it
	glutCreateMenu(mainMenu);
	glutAddSubMenu("Add Model", menuAddModel);

	//add these menus only if we have models in our scene
	if (gScene->getModels().size() != 0) {
		glutAddSubMenu("Select Model", menuSelectModel);
		glutAddSubMenu("Active Model Options", menuActiveModelOptions);
	}

	glutAddMenuEntry("Add Camera", ADD_CAMERA);
	glutAddSubMenu("Select Camera", menuSelectCamera);
	glutAddSubMenu("Active Camera Options", menuActiveCameraOptions);
	glutAddSubMenu("Add Light", menuAddLight);
	glutAddSubMenu("Select Light", menuSelectLight);
	glutAddSubMenu("Active Light Options", menuActiveLightOptions);

	//glutAddMenuEntry("Demo", MAIN_DEMO);
	glutAddMenuEntry("Help", HELP);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//----------------------------------------------------------------------------

