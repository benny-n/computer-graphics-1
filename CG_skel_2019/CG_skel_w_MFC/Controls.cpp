#include "stdafx.h"
#include "Controls.h"
#include<limits>

using namespace std;

int gLastX, gLastY;
bool gLbDown, gRbDown, gMbDown;
extern Scene* gScene;
extern Renderer* gRenderer;


float getFloatFromUser(const string& directive)
{
	float input = -1;
	cout << directive << ": ";
	cin >> input;
	while (!cin || input < 0 || input > 1) {
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
	//cout << "pressed key: " << int(key) << " on x: "  << x << " and y: " << y << endl;
	switch (key) {
	case 033:
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
	case '\t':
		gScene->iterateActive();
		break;
	case ' ':
		gScene->toggleControlCamera();
		break;
	case 23:
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
	switch (id)
	{
	case FILE_OPEN:
		CFileDialog dlg(TRUE, _T(".obj"), NULL, NULL, _T("*.obj|*.*"));
		if (dlg.DoModal() == IDOK)
		{
			std::string s((LPCTSTR)dlg.GetPathName());
			gScene->loadOBJModel((LPCTSTR)dlg.GetPathName());
			gScene->mActiveModel = gScene->getModels().size() - 1;
			gScene->setControlCamera(false);
			glutPostRedisplay();
			initMenu();
		}
		break;
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
	gScene->mActiveModel = gScene->getModels().size() - 1;
	gScene->setControlCamera(false);
	glutPostRedisplay();
	initMenu();
}

void selectModelMenu(int id) {
	gScene->mActiveModel = id;
}

void changeColorMenu(int id) {
	switch (id)
	{
	case WHITE:
		gScene->changeColor(vec3(1));
		break;
	case RED:
		gScene->changeColor(vec3(1, 0, 0));
		break;
	case GREEN:
		gScene->changeColor(vec3(0, 1, 0));
		break;
	case BLUE:
		gScene->changeColor(vec3(0, 0, 1));
		break;
	case YELLOW:
		gScene->changeColor(vec3(1, 1, 0));
		break;
	case VISUALIZE_SLOPES:
		gScene->visualizeSlopes();
		break;
	case CUSTOM_COLOR:
		inputMessage();
		cout << "Please enter rgb values between 0 and 1" << endl;
		float r = getFloatFromUser("r");
		float g = getFloatFromUser("g");
		float b = getFloatFromUser("b");
		cout << "Custom color: (" << r << ", " << g << ", " << b << ")" << endl;
		gScene->changeColor(vec3(r, g, b));
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
	case REMOVE_ACTIVE_MODEL:
		gScene->removeActiveModel();
		initMenu();
		break;
	}
	glutPostRedisplay();
}

void mainMenu(int id)
{
	switch (id)
	{
	//case MAIN_DEMO:
	//	gScene->drawDemo();
	//	break;
	case MAIN_ABOUT:
		AfxMessageBox(_T("Computer Graphics"));
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

	//create change color menu
	int menuChangeColor = glutCreateMenu(changeColorMenu);
	glutAddMenuEntry("White", WHITE);
	glutAddMenuEntry("Red", RED);
	glutAddMenuEntry("Green", GREEN);
	glutAddMenuEntry("Blue", BLUE);
	glutAddMenuEntry("Yellow", YELLOW);
	glutAddMenuEntry("Visualize Slopes", VISUALIZE_SLOPES);
	glutAddMenuEntry("Choose custom color", CUSTOM_COLOR);

	//create active model options menu
	int menuActiveModelOptions = glutCreateMenu(activeModelOptionsMenu);
	glutAddMenuEntry("Plot Boundry Box", PLOT_BOUNDRY_BOX);
	glutAddMenuEntry("Plot Vertex Normals", PLOT_VERTEX_NORMALS);
	glutAddMenuEntry("Plot Face Normals", PLOT_FACE_NORMALS);
	glutAddSubMenu("Change Color", menuChangeColor);
	glutAddMenuEntry("Remove Active Model", REMOVE_ACTIVE_MODEL);

	//finally, create the main menu and start adding submenus to it
	glutCreateMenu(mainMenu);
	glutAddSubMenu("Add Model", menuAddModel);

	//add these menus only if we have models in our scene
	if (gScene->getModels().size() != 0) {
		glutAddSubMenu("Select Model", menuSelectModel);
		glutAddSubMenu("Active Model Options", menuActiveModelOptions);
	}

	//glutAddMenuEntry("Demo", MAIN_DEMO);
	glutAddMenuEntry("About", MAIN_ABOUT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//----------------------------------------------------------------------------

