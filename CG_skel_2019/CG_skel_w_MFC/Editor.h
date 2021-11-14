#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "resource.h"

#define MOUSE_WHEEL_UP 3
#define MOUSE_WHEEL_DOWN 4

// defines for menus
#define FILE_OPEN 1
#define MAIN_DEMO 1
#define MAIN_ABOUT 2
#define SUCCESS 0

#define FROM_FILE 1
#define PRIM 2

#define CUBE 1
#define PYRAMID 2

// define for sensetivity
#define SENSETIVITY 0.2 //BONUS if we want sensetivity to be interactive, make it a variable
#define SCALE_UP (1 + SENSETIVITY)
#define SCALE_DOWN (1 / SCALE_UP)
#define ROTATE (60 * SENSETIVITY)
#define TRANSLATE (SENSETIVITY)


void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void fileMenu(int id);
void mainMenu(int id);
void initMenu();