#include "stdafx.h"
#include "Editor.h"

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
	renderer->Reshape(width, height);
	scene->draw();
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
		status = scene->transformActiveModel(Scale(1.1));
		break;
	case MOUSE_WHEEL_DOWN:
		status = scene->transformActiveModel(Scale(0.9));
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
	glutPostRedisplay();
	initMenu();
}

void selectModelMenu(int id) {
	scene->activeModel = id;
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
		string x;
		cout << "input please: ";
		cin >> x;
		cout << "x is " + x << endl;
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

	//create active model options menu
	int menuActiveModelOptions = glutCreateMenu(activeModelOptionsMenu);
	glutAddMenuEntry("Plot Boundry Box", PLOT_BOUNDRY_BOX);
	glutAddMenuEntry("Plot Vertex Normals", PLOT_VERTEX_NORMALS);
	glutAddMenuEntry("Plot Face Normals", PLOT_FACE_NORMALS);

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

