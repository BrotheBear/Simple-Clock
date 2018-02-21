// Define the header definition.
#ifndef CLOCKHEADER_H
#define CLOCKHEADER_H

// Include the files needed for the project.
#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>
#include "glm.h" 
#include <iostream>
#include <time.h>
#include <sstream>
#include <math.h>
using namespace std;
// Create the colour structure.
struct colour{
	float r;
	float g;
	float b;
};
// Create a structure to hold X Y Z co-ordinates.
struct point3D {
	float x;
	float y;
	float z;
};
// Create a camera structure.
struct camera {
	point3D pos;
	point3D lookAt;
	point3D up;
};
// Create a lighting structure.
struct lighting
{
	 float ambient[4];
	 float diffuse[4];
	 float specular[4];
};
// Create the standard white lighting.
// This light does not change.
lighting clockWhiteLighting = 
{ 
	{0.1, 0.1, 0.1, 1.0},
	{2, 2, 2, 1.0},
	{2, 2, 2, 1.0}
};
// Create the coloured light.
// This is the light that changes colour.
lighting clockColouredLighting = 
{ 
	{0.1, 0, 0, 1.0},
	{1, 0, 0, 1.0},
	{1, 0, 0, 1.0}
};
// Create the ambient light.
// This light doesn't change, and is used to brighten the
// scene a little so that fraps can easily pick up all the models.
lighting ambientLighting =
{
	{0.5,0.5,0.5,1},
	{0,0,0,0},
	{0,0,0,0}
};
// Create definitions, to enable easy use of integers without having
// to remember what each integer is defined as.
#define UPARROW 101
#define DOWNARROW 103 
#define LEFTARROW 100
#define RIGHTARROW 102
#define TRUE 1
#define FALSE 0
// Create a new type definition called Boolean, defined as an integer.
typedef int BOOL;

// Create all the float variables.
float sHandR = 0, lHandR = 0;
float lHandRSpeed, sHandRSpeed;
// Create the two float arrays for the light positions.
float light0_pos[] =  {-2,2,0.4, 0};
float light1_pos[] =  {0,0,1, 0};

// Create the integer values that are used in this program.
int lastX, lastY;
int cameraPos = 0;
int start_t;
int speedLevel = 1;
int lastHour = 0;

// Create the BOOL values.
// These could quite easily have been declared as integers, 
// or bool values, however I wanted to test to ensure I could,
// create a working typedef.
BOOL hourPM = FALSE;
BOOL showControls = FALSE;
BOOL showMouseControls = FALSE;
BOOL lmbDown = FALSE;
BOOL rmbDown = FALSE;
BOOL colouredLight = FALSE;
// Initalise the camera.
camera cam = {0, 0, 1, 0, 0, 0, 0, 1, 0};
// Initalise the colour array, which is used to modify the
// coloured lighting structure.
colour col = {1,0,0};
// Create the GLM model variables used to hold the objects in the scene.
GLMmodel * clock_M;
GLMmodel * lHand_M;
GLMmodel * sHand_M;

// Declare the function prototypes to allow 
// use of the functions in any order.
void adjustLightColour(int direction);
void animationFunction();
void boundsCheckf(float &i, float x, float y, BOOL loop, float decimal);
void boundsChecki(int &i, int x, int y, int loop);
string currentTime();
void displayFunction(void);
void initialise(void);
void keyboardFunction(unsigned char key, int x, int y);
void keyboardSpecialFunction(int key, int x, int y);
void loadModels();
int main(int argc, char** argv);
void motionFunction(int x, int y);
void mouseFunction(int b, int s, int x, int y);
void outputText(int x, int y, string t, int size);
void renderHUD();
void reshapeFunction(int w, int h);
void setLight(lighting * light, int GL_LIGHT, float pos[]);
void updateSpeed();

#endif