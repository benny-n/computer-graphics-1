// CG_skel_w_MFC.cpp : Defines the entry point for the console application.
//

#include "CG_skel_w_MFC.h"
#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object
Scene *gScene;
Renderer *gRenderer;

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
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
	
	
	gRenderer = new Renderer(512,512);
	gScene = new Scene(gRenderer);
	//----------------------------------------------------------------------------
	// Initialize Callbacks

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc (motion);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	initMenu();
	

	glutMainLoop();
	delete gScene;
	delete gRenderer;
	return 0;
}

CWinApp theApp;

using namespace std;

int sanityTests() {
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

	////int nRetCode = sanity_tests();
	int nRetCode = 0;
	
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		my_main(argc, argv );
	}
	
	return nRetCode;
}
