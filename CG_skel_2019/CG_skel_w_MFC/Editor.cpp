#include "stdafx.h"
#include "Editor.h"
#include<limits>

using namespace std;

int last_x, last_y;
bool lb_down, rb_down, mb_down;
extern Scene* scene;
extern Renderer* renderer;

void handle_err_code(int err) {
	switch (err) {
	case -1:
		AfxMessageBox(_T("Add a model before transforming!"));
		break;
	default:
		break;
	}
}

void inputMessage() {
	AfxMessageBox(_T("Please enter your wanted parameters in the console window"));
}


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

//----------------------------------------------------------------------------
// Callbacks
void display(void)
{
	//TODO
//Call the scene and ask it to draw itself
	scene->draw();
}

void reshape(int width, int height)
{
	//debug
	//cout << width << endl;
	//cout << height << endl;
	////TODO
	//cout << "reshape" << endl;
	const vec2 screenSize = renderer->GetScreenSize();
	for each (auto model in scene->getModels()) {
		model->transform(Scale(screenSize.x / width, screenSize.y / height, 1));
	}
	renderer->Reshape(width, height);
	scene->draw();
	//glViewport(0, 0, width, height);

//update the renderer's buffers
}

void keyboard(unsigned char key, int x, int y)
{
	int status = 0;
	//cout << "pressed key: " << key << " on x: "  << x << " and y: " << y << endl;
	switch (key) {
	case 033:
		exit(EXIT_SUCCESS);
		break;	
	case 'z':
		status = scene->transformActiveModel(Scale(SCALE_UP));
		break;
	case 'x':
		status = scene->transformActiveModel(Scale(SCALE_DOWN));
		break;
	case 'D':
		status = scene->transformActiveModel(RotateY(ROTATE));
		break;
	case 'A':
		status = scene->transformActiveModel(RotateY(-ROTATE));
		break;	
	case 'W':
		status = scene->transformActiveModel(RotateX(ROTATE));
		break;
	case 'S':
		status = scene->transformActiveModel(RotateX(-ROTATE));
		break;
	case 'Q':
		status = scene->transformActiveModel(RotateZ(ROTATE));
		break;
	case 'E':
		status = scene->transformActiveModel(RotateZ(-ROTATE));
		break;
	case 'd':
		status = scene->transformActiveModel(Translate(TRANSLATE, 0, 0));
		break;	
	case 'a':
		status = scene->transformActiveModel(Translate(-TRANSLATE, 0, 0));
		break;
	case 'w':
		status = scene->transformActiveModel(Translate(0, TRANSLATE, 0));
		break;
	case 's':
		status = scene->transformActiveModel(Translate(0, -TRANSLATE, 0));
		break;
	case 'q':
		status = scene->transformActiveModel(Translate(0, 0, TRANSLATE));
		break;
	case 'e':
		status = scene->transformActiveModel(Translate(0, 0, -TRANSLATE));
		break;
	case 'c':
		scene->toggleRenderCameras();
		break;
	case '\t':
		scene->iterateModels();
		break;

	default:
		return;
	}

	//handle status
	if (status != SUCCESS) {
		handle_err_code(status);
	}
	else {
		glutPostRedisplay();
	}
}

void mouse(int button, int state, int x, int y)
{
	//button = {GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON}
	//state = {GLUT_DOWN,GLUT_UP}
	int status = 0;
	//set down flags
	switch (button) {
	case GLUT_LEFT_BUTTON:
		lb_down = (state == GLUT_UP) ? 0 : 1;
		break;
	case GLUT_RIGHT_BUTTON:
		rb_down = (state == GLUT_UP) ? 0 : 1;
		break;
	case GLUT_MIDDLE_BUTTON:
		mb_down = (state == GLUT_UP) ? 0 : 1;
		break;
	case MOUSE_WHEEL_UP:
		status = scene->transformActiveModel(Scale(SCALE_UP));
		break;
	case MOUSE_WHEEL_DOWN:
		status = scene->transformActiveModel(Scale(SCALE_DOWN));
		break;
	}

	//TODO
	// add your code
	if (status != SUCCESS) {
		handle_err_code(status);
	}
	else {
		scene->draw();
	}
}

void motion(int x, int y)
{
	// calc difference in mouse movement
	int dx = x - last_x;
	int dy = y - last_y;
	// update last x,y
	last_x = x;
	last_y = y;
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
			scene->loadOBJModel((LPCTSTR)dlg.GetPathName());
			scene->activeModel = scene->getModels().size() - 1;
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
		scene->loadCubeModel();
		break;
	case PYRAMID:
		scene->loadPyramidModel();	
		break;
	}
	scene->activeModel = scene->getModels().size() - 1;
	glutPostRedisplay();
	initMenu();
}

void selectModelMenu(int id) {
	scene->activeModel = id;
}

void changeColorMenu(int id) {
	switch (id)
	{
	case WHITE:
		scene->changeColor(vec3(1));
		break;
	case RED:
		scene->changeColor(vec3(1, 0, 0));
		break;
	case GREEN:
		scene->changeColor(vec3(0, 1, 0));
		break;
	case BLUE:
		scene->changeColor(vec3(0, 0, 1));
		break;
	case YELLOW:
		scene->changeColor(vec3(1, 1, 0));
		break;
	case VISUALIZE_SLOPES:
		scene->visualizeSlopes();
		break;
	case CUSTOM_COLOR:
		inputMessage();
		cout << "Please enter rgb values between 0 and 1" << endl;
		float r = getFloatFromUser("r");
		float g = getFloatFromUser("g");
		float b = getFloatFromUser("b");
		cout << "Custom color: (" << r << ", " << g << ", " << b << ")" << endl;
		scene->changeColor(vec3(r, g, b));
		break;
	
	}
	glutPostRedisplay();
}

void activeModelOptionsMenu(int id) {

	switch (id)
	{
	case PLOT_BOUNDRY_BOX:
		scene->togglePlotBoundryBox();
		break;
	case PLOT_VERTEX_NORMALS:
		scene->togglePlotVertexNormals();
		break;
	case PLOT_FACE_NORMALS:
		scene->togglePlotFaceNormals();
		break;
	}
	glutPostRedisplay();
}

void mainMenu(int id)
{
	switch (id)
	{
	case MAIN_DEMO:
		scene->drawDemo();
		break;
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
	for each (auto model in scene->getModels()) {
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

	//finally, create the main menu and start adding submenus to it
	glutCreateMenu(mainMenu);
	glutAddSubMenu("Add Model", menuAddModel);

	//add these menus only if we have models in our scene
	if (scene->getModels().size() != 0) {
		glutAddSubMenu("Select Model", menuSelectModel);
		glutAddSubMenu("Active Model Options", menuActiveModelOptions);
	}

	glutAddMenuEntry("Demo", MAIN_DEMO);
	glutAddMenuEntry("About", MAIN_ABOUT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//----------------------------------------------------------------------------

