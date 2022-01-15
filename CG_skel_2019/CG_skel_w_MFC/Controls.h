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

// change texture coords menu
#define PROJECTION_ON_PLAIN 1
#define PROJECTION_ON_CYLINDER 2
#define TBD_TEX_COORDS 3

// change texture menu
#define NO_TEX 1
#define WOOD 2

// change normal map menu
#define	NO_NORMAL_MAP 1	

// active camera options menu
#define FOCUS 1
#define ORTHO 2
#define FRUSTUM 3
#define PERSPECTIVE 4
#define LOOK_AT 5
#define MOVE_CAMERA_TO 6
#define REMOVE_ACTIVE_CAMERA 7

// add light menu
#define POINT 1
#define PARALLEL 2

// active light options menu
#define MOVE_LIGHT_TO 1
#define CHANGE_DIRECTION_TO 2
#define REMOVE_ACTIVE_LIGHT 3

// pick rasterizer menu
#define FLAT 1
#define GOURAUD 2
#define PHONG 3

// animations menu
#define HULK_OUT 1

// color animation menu
#define INVERT_COLORS 1
#define BANANIFY 2

// skybos menu
#define OCEAN 1
#define NO_SKYBOX 2


// define for sensetivity
#define SCALE_UP (1 + 0.2 * gSensetivity)
#define SCALE_DOWN (1.0 / SCALE_UP)
#define ROTATE (12 * gSensetivity)
#define TRANSLATE (0.2 * gSensetivity)

void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void special(int key, int x, int y);
void motion(int x, int y);
void fileMenu(int id);
void addPrimMenu(int id);
void selectModelMenu(int id);
void changeMaterialMenu(int id);
void changeModelColorMenu(int id);
void changeTexCoordsMenu(int id);
void texFileMenu(int id);
void changeTexMenu(int id);
void normalMapFileMenu(int id);
void changeNormalMapMenu(int id);
void activeModelOptionsMenu(int id);
void selectCameraMenu(int id);
void activeCameraOptionsMenu(int id);
void selectLightMenu(int id);
void changeLightColorMenu(int id);
void activeLightOptionsMenu(int id);
void pickRasterizerMenu(int id);
void animationsMenu(int id);
void colorAnimationsMenu(int id);
void changeSkyboxMenu(int id);
void mainMenu(int id);
void initMenu();