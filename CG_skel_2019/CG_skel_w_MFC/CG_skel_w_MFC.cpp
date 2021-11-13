// CG_skel_w_MFC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "vec.h"
#include "mat.h"
#include "InitShader.h"
#include "Scene.h"
#include "Renderer.h"
#include <string>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#define MOUSE_WHEEL_UP 3
#define MOUSE_WHEEL_DOWN 4


// defines for menus
#define FILE_OPEN 1
#define MAIN_DEMO 1
#define MAIN_ABOUT 2


#define FROM_FILE 1
#define PRIM 2

#define CUBE 1
#define PYRAMID 2

Scene *scene;
Renderer *renderer;

int last_x,last_y;
bool lb_down,rb_down,mb_down;

//----------------------------------------------------------------------------
// Callbacks

void display( void )
{
	//TODO
//Call the scene and ask it to draw itself
	scene->draw();
}

void reshape( int width, int height )
{
	//TODO
//update the renderer's buffers
}

void keyboard( unsigned char key, int x, int y )
{
	//BONUS
	switch ( key ) {
	case 033:
		exit( EXIT_SUCCESS );
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	//button = {GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON}
	//state = {GLUT_DOWN,GLUT_UP}
	
	//set down flags
	switch(button) {
		case GLUT_LEFT_BUTTON:
			lb_down = (state==GLUT_UP)?0:1;
			break;
		case GLUT_RIGHT_BUTTON:
			rb_down = (state==GLUT_UP)?0:1;
			break;
		case GLUT_MIDDLE_BUTTON:
			mb_down = (state==GLUT_UP)?0:1;	
			break;
		case MOUSE_WHEEL_UP:
			cout << "rolling up" << endl;
			break;
		case MOUSE_WHEEL_DOWN:
			cout << "rolling down" << endl;
			break;
	}
	//TODO
	// add your code
}

void motion(int x, int y)
{
	// calc difference in mouse movement
	int dx=x-last_x;
	int dy=y-last_y;
	// update last x,y
	last_x=x;
	last_y=y;
}

void fileMenu(int id)
{
	switch (id)
	{
		case FILE_OPEN:
			CFileDialog dlg(TRUE,_T(".obj"),NULL,NULL,_T("*.obj|*.*"));
			if(dlg.DoModal()==IDOK)
			{
				std::string s((LPCTSTR)dlg.GetPathName());
				scene->loadOBJModel((LPCTSTR)dlg.GetPathName());
				scene->draw();
			}
			break;
	}
}

void addPrimMenu(int id) {
	switch (id)
	{
	case CUBE:
		scene->loadCubeModel();
		scene->draw();
		break;
	case PYRAMID:
		scene->loadPyramidModel();
		scene->draw();
		break;
	}
}

void addModelMenu(int id) {
	switch (id)
	{
	case FROM_FILE:
		break;
	case PRIM:
		break;
	}
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
	int menuFile = glutCreateMenu(fileMenu);
	glutAddMenuEntry("Open..",FILE_OPEN);

	int menuAddPrim = glutCreateMenu(addPrimMenu);
	glutAddMenuEntry("Cube", CUBE);
	glutAddMenuEntry("Pyramid", PYRAMID);

	int menuAddModel = glutCreateMenu(addModelMenu);
	glutAddSubMenu("From File", menuFile);
	glutAddSubMenu("Add Primitive Model", menuAddPrim);
	
	glutCreateMenu(mainMenu);
	glutAddSubMenu("Add Model", menuAddModel);
	glutAddMenuEntry("Demo",MAIN_DEMO);
	glutAddMenuEntry("About",MAIN_ABOUT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//----------------------------------------------------------------------------



int my_main( int argc, char **argv )
{
	//----------------------------------------------------------------------------
	// Initialize window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA| GLUT_DOUBLE);
	glutInitWindowSize( 512, 512 );
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutCreateWindow( "CG" );
	glewExperimental = GL_TRUE;
	glewInit();
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		/*		...*/
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	
	
	renderer = new Renderer(512,512);
	scene = new Scene(renderer);
	//----------------------------------------------------------------------------
	// Initialize Callbacks

	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mouse );
	glutMotionFunc ( motion );
	glutReshapeFunc( reshape );
	initMenu();
	

	glutMainLoop();
	delete scene;
	delete renderer;
	return 0;
}

CWinApp theApp;

using namespace std;

int sanity_tests() {
	mat4 a = mat4(
		vec4(1, 2, 3, 4),
		vec4(5, 6, 7, 8),
		vec4(9, 10, 11, 12),
		vec4(13, 14, 15, 16)
	);
	mat3 b = mat3(
		vec3(1, 2, 3),
		vec3(4, 5, 6),
		vec3(7, 8, 9)
	);
	mat2 c = mat2(
		vec2(1, 2),
		vec2(3, 4)
	);
	a = transpose(a);
	b = transpose(b);
	c = transpose(c);
	std::cout << a << std::endl;
	std::cout << b << std::endl;
	std::cout << c << std::endl;
	return 0;
}

int main( int argc, char **argv )
{
	int nRetCode = sanity_tests();
	
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		my_main(argc, argv );
	}
	
	return nRetCode;
}
