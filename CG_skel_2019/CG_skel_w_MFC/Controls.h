#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "resource.h"

#define MOUSE_WHEEL_UP 3
#define MOUSE_WHEEL_DOWN 4

// defines for menus
// main menu
#define ADD_CAMERA 1
#define MAIN_DEMO 2
#define HELP 3

// add model menu
#define FROM_FILE 1
#define PRIM 2

// open file menu
#define FILE_OPEN 1

// add primitive menu
#define CUBE 1
#define PYRAMID 2

// active model options menu
#define PLOT_BOUNDRY_BOX 1
#define PLOT_VERTEX_NORMALS 2
#define PLOT_FACE_NORMALS 3
#define MOVE_MODEL_TO 4
#define REMOVE_ACTIVE_MODEL 5

// change material menu
#define UNIFORM_MATERIAL 1
#define NON_UNIFORM_MATERIAL 2

// change color menu
#define WHITE 1
#define RED 2
#define GREEN 3
#define BLUE 4
#define YELLOW 5
#define CUSTOM_COLOR 6

// active camera options menu
#define FOCUS 1
#define ORTHO 2
#define FRUSTUM 3
#define PERSPECTIVE 4
#define LOOK_AT 5
#define MOVE_CAMERA_TO 6
#define REMOVE_ACTIVE_CAMERA 7

// define for sensetivity

#define SCALE_UP (1 + 0.2 * gSensetivity)
#define SCALE_DOWN (1.0 / SCALE_UP)
#define ROTATE (12 * gSensetivity)
#define TRANSLATE (0.2 * gSensetivity)

float getFloatFromUser(const string& directive, bool wantFraction = false);
void inputMessage();
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void special(int key, int x, int y);
void motion(int x, int y);
void fileMenu(int id);
void addPrimMenu(int id);
void selectModelMenu(int id);
void changeColorMenu(int id);
void activeModelOptionsMenu(int id);
void selectCameraMenu(int id);
void activeCameraOptionsMenu(int id);
void mainMenu(int id);
void initMenu();