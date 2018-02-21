#include <stdafx.h>

// Include the header file with all the structures, variables
// and function prototypes.
#include "ClockHeader.h"

void adjustLightColour(int direction)
{
	// Increase the colour value of the coloured lighting.
	// Red - Green - Blue.
	if(direction)
	{
		if(col.r && col.g < 1 && !col.b)
		{	col.r -= 0.05;	col.g += 0.05;	}
		else if(!col.r && col.g && col.b < 1)
		{	col.g -= 0.05;	col.b += 0.05;	}
		else if(col.r < 1 && !col.g && col.b)
		{	col.b -= 0.05;	col.r += 0.05;	}
		
	}
	// Decrease the colour value of the coloured lighting.
	// Red - Blue - Green.
	else
	{
		if(col.r && !col.g && col.b < 1)
		{	col.r -= 0.05;	col.b += 0.05;	}
		else if(!col.r && col.g < 1 && col.b)
		{	col.b -= 0.05;	col.g += 0.05;	}
		else if(col.r < 1 && col.g && !col.b)
		{	col.g -= 0.05;	col.r += 0.05;	}
		
	}
	// Make sure that the colours are still inside the proper range,
	// and truncate it to two decimal places.
	boundsCheckf(col.r, 0, 1, 0, 2);
	boundsCheckf(col.g, 0, 1, 0, 2);
	boundsCheckf(col.b, 0, 1, 0, 2);
	// Modify the coloured lighting structure:
	// - Set the ambient lighting to 1/10th of the colour value.
	clockColouredLighting.ambient[0] = col.r/10;
	clockColouredLighting.ambient[1] = col.g/10;
	clockColouredLighting.ambient[2] = col.b/10;
	// - Set the diffuse lighting to the specified colour value.
	clockColouredLighting.diffuse[0] = col.r;
	clockColouredLighting.diffuse[1] = col.g;
	clockColouredLighting.diffuse[2] = col.b;
	// - Set the specular lighting to the specified colour value.
	clockColouredLighting.specular[0] = col.r;
	clockColouredLighting.specular[1] = col.g;
	clockColouredLighting.specular[2] = col.b;
	

	setLight(&clockColouredLighting, GL_LIGHT0, light0_pos);
}

void animationFunction()
{
	// Pause for 100th of a second. The program doesn't need to run at full proccessor speed.
	Sleep(10);
	// A crude Frame Limiting method. Wait for a gap of 30 between the clock before drawing
	// the next frame.
	if(clock() - start_t >= 30 )
	{
		// Reset the gap between the clock speed.
		start_t = clock();
		// Rotate the hands by the specified speed.
		lHandR -= lHandRSpeed;
		sHandR -= sHandRSpeed;
		// Check to make sure clocks are within bounds. 
		// If out of bounds, reset the rotation. EG if the rotation reaches 360,
		// set it back to 0.
		boundsCheckf(lHandR, -359, 0, 1, 0);
		boundsCheckf(sHandR, -359, 0, 1, 0);
		// Redraw the screen.
		glutPostRedisplay();
	}
}

void boundsCheckf(float &i, float x, float y, BOOL loop, float decimal)
{
	// If a decimal amount is specificed, truncate to that many decimal places.
	if(decimal)	i = floor(i * pow(10,decimal)) / pow(10,decimal); 
	// If loop is specified, once one of the boundaries is reach, set the value of 
	// i to the opposite boundary.
	if(loop) 
	{
		if(i < x) i = y;
		if(i > y) i = x;
	}
	// If the loop is unspecified, once one of the boundaries is reached, stop the value of I
	// moving in that direction further.
	else
	{
		if(i < x) i = x;
		if(i > y) i = y;
	}	
}

void boundsChecki(int &i, int x, int y, int loop)
{
	// If loop is specified, once one of the boundaries is reach, set the value of 
	// i to the opposite boundary.
	if(loop) 
	{
		if(i < x) i = y;
		if(i > y) i = x;
	}
	// If the loop is unspecified, once one of the boundaries is reached, stop the value of I
	// moving in that direction further.
	else
	{
		if(i < x) i = x;
		if(i > y) i = y;
	}	
}

string currentTime()
{
	// Create a string stream.
	stringstream ss;
	// Calculate the current position in hours the small hand is currently at.
	int sHand = floor(sHandR / 30) + 1;
	sHand = sHand - (sHand)*2;
	// Calculate the current position in minutes the large hand is currently at.
	int lHand = floor(lHandR / 6) + 1;
	lHand = lHand - (lHand)*2;

	// Check the boundaries for the calculated time.
	boundsChecki(sHand, 0, 11, 1);
	boundsChecki(lHand, 0, 59, 1);

	// If the time reaches 12am or 12pm, toggle variable hourPM.
	if(lastHour == 11 && sHand == 0) { hourPM = !hourPM; }

	// Set the last hour to the current hour. Used for the AM/PM Calculation.
	lastHour = sHand;

	// If small hand has passed 12 once, add 12 to the output.
	if(hourPM) ss << sHand + 12;
	// If the small hand number is below 10, add a 0 prefix.
	else if(sHand < 10) ss << "0" << sHand;
	// If not, just output the small hand number properly.
	else ss << sHand;
	// Add a clock separator.
	ss << ":";

	// If the large hand number is below 10, add a 0 prefix.
	if(lHand < 10) ss << "0" << lHand;
	// Else, add the large hand number to the string stream.
	else ss << lHand;

	// Return the string value of the stringstream.
	return ss.str();
}

void displayFunction(void)
{
	// Clear the current buffers.
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Load the current matrix.
	glLoadIdentity ();
	// Move the entire scene away from the camera.
	glTranslatef(0,0,-4);	
	// Position the camera.
	gluLookAt(cam.pos.x, cam.pos.y, cam.pos.z,
	  cam.lookAt.x, cam.lookAt.y, cam.lookAt.z, 
	  cam.up.x, cam.up.y, cam.up.z);

	// Draw the clock in its own matrix.
	glPushMatrix();		
		glmDraw(clock_M, GLM_SMOOTH| GLM_TEXTURE);	
	glPopMatrix();

	// Draw the small hand in its own matrix.
	glPushMatrix();
		// Rotate the small clock hand to the correct position.
		glRotatef(sHandR,0,0,1);
		glmDraw(sHand_M, GLM_SMOOTH| GLM_TEXTURE);	
	glPopMatrix();

	// Draw the large hand in its own matrix.
	glPushMatrix();
		// Rotate the large clock hand to the correct position.
		glRotatef(lHandR,0,0,1);
		// Move the large clock hand slightly away from the smaller hand,
		// to prevent overlaps.
		glTranslatef(0,0,0.05);
		glmDraw(lHand_M, GLM_SMOOTH| GLM_TEXTURE);	
	glPopMatrix();
	
	// Disable the lighting, to prevent the HUD from becoming coloured.
	glDisable(GL_LIGHTING);
	// Draw the HUD.
	renderHUD();
	// Re-enable the lighting.
	glEnable(GL_LIGHTING);

	// Swap the buffers. Everything that was just drawn to the back
	// buffer, display on the screen, and prepare to draw the next frame.
	glutSwapBuffers();
}

void initialise(void) 
{
	// Set the background to black.
	glClearColor (0.0, 0.0, 0.0, 0.0);  
	// Enable depth testing and smooth models.
	glEnable(GL_DEPTH_TEST);
	glShadeModel (GL_SMOOTH);
	// Load the models.
	loadModels();
	// Initialise the current default clock speed.
	updateSpeed();
	// Enable lighting, and create two lights.
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	// Set the matrix mode to model view.
	glMatrixMode(GL_MODELVIEW);
	// Set the first light to a standard white light.
	setLight(&clockWhiteLighting, GL_LIGHT0, light0_pos);
	// Set the second light to a mild ambient light.
	setLight(&ambientLighting, GL_LIGHT1, light1_pos);
}

void keyboardFunction(unsigned char key, int x, int y)
{
	switch (key)
	{	// If the - key is pressed, reduce the clock speed and update the speeds.
		case '-': speedLevel --; updateSpeed(); break;
		// If the = key is pressed, increase the clock speed and update the speeds.
		case '=': speedLevel ++; updateSpeed(); break;
		// If the h key is pressed, toggle the keyboard controls.
		case 'h': showControls = !showControls; 
					showMouseControls = FALSE; break;
		// If the m key is pressed, toggle the mouse controls.
		case 'm': showMouseControls = !showMouseControls; 
					showControls = FALSE; break;
		// If the l key is pressed, toggle the light colour.
		case 'l': colouredLight = !colouredLight; 
		colouredLight ? setLight(&clockColouredLighting, GL_LIGHT0, light0_pos) :
					setLight(&clockWhiteLighting, GL_LIGHT0, light0_pos); break;
   }
}

void keyboardSpecialFunction(int key, int x, int y)
{
	// If the right arrow is pressed.
	if(key == RIGHTARROW)
	{
		// Move light 0 to the right.
		light0_pos[0] += 0.05;
		// Check to see if the light is still within the specified boundaries.
		boundsCheckf(light0_pos[0], -3, 3, 0, 2);
		// Update the light position, using the correct light colour.
		colouredLight ? setLight(&clockColouredLighting, GL_LIGHT0, light0_pos) :
			setLight(&clockWhiteLighting, GL_LIGHT0, light0_pos);
	}
	// If the left arrow is pressed.
	else if(key == LEFTARROW)
	{
		// Move light 0 to the left.
		light0_pos[0] -= 0.05;
		// Check to see if the light is still within the specified boundaries.
		boundsCheckf(light0_pos[0], -3, 3, 0, 2);
		// Update the light position, using the correct light colour.
		colouredLight ? setLight(&clockColouredLighting, GL_LIGHT0, light0_pos) :
			setLight(&clockWhiteLighting, GL_LIGHT0, light0_pos);
	}
	// If coloured lighting is enabled.
	// This prevents the light colour being changed while the light is white.
	if(colouredLight)
	{
		// If the up arrow is pressed.
		if(key == UPARROW)
		{
			// Adjust the light colour.
			adjustLightColour(1);
		}
		// If the down arrow is pressed.
		else if(key == DOWNARROW)
		{
			// Adjust the light colour.
			adjustLightColour(0);
		}
	}

}

void loadModels()
{
	// If the clock model isn't loaded, ...
	if (!clock_M) {
		// ... load it.
	    clock_M = glmReadOBJ("3D Models/Clock.obj");	
		// If the model cannot be found, exit the program.
	    if (!clock_M) exit(0);
		// Setup the faces of the model.
		glmFacetNormals(clock_M);       
		// Setup the lighting co-ordinates of the model, with a
		// reflection angle of 90 degrees.
		glmVertexNormals(clock_M, 90.0);
    }
	// If the large hand model isn't loaded, ...
	if (!lHand_M) {
		// ... load it.
	    lHand_M = glmReadOBJ("3D Models/LargeHand.obj");	
		// If the model cannot be found, exit the program.
	    if (!lHand_M) exit(0);
		// Setup the faces of the model.
		glmFacetNormals(lHand_M);       
		// Setup the lighting co-ordinates of the model, with a
		// reflection angle of 90 degrees.
		glmVertexNormals(lHand_M, 90.0);
    }
	// If the small hand model isn't loaded, ...
	if (!sHand_M) {
		// ... load it.
	    sHand_M = glmReadOBJ("3D Models/SmallHand.obj");	
		// If the model cannot be found, exit the program.
	    if (!sHand_M) exit(0);
		// Setup the faces of the model.
		glmFacetNormals(sHand_M);       
		// Setup the lighting co-ordinates of the model, with a 
		// reflection angle of 90 degrees.
		glmVertexNormals(sHand_M, 90.0);
    }    
}

int main(int argc, char** argv)
{
	// Initialise the glut controls.
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	// Setup the window size and position.
	glutInitWindowSize (600, 600); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	// Launch the startup function.
	initialise();
	// Assign the display function.
	glutDisplayFunc(displayFunction); 
	// Assign the reshape function.
	glutReshapeFunc(reshapeFunction);
	// Assign the keyboard function.
	glutKeyboardFunc(keyboardFunction);
	// Assign the special key function.
	glutSpecialFunc(keyboardSpecialFunction);
	// Assign the mouse function.
	glutMouseFunc(mouseFunction);
	// Assign the motion function.
	glutMotionFunc(motionFunction);
	// Assign the animation function.
	glutIdleFunc(animationFunction);
	// Begin the program.
	glutMainLoop();
	return 0;
}

void motionFunction(int x, int y)
{
	// If the left and right mouse buttons are held.
	if(lmbDown && rmbDown)
	{
		// If the mouse is dragged across the Y axis, zoom in and out.
		y < lastY ? cam.pos.z -= 0.1 : cam.pos.z += 0.1;
		// Check to make sure the camera moves only in the specified boundaries.
		boundsCheckf(cam.pos.z, 1, 10, 0, 2);
	}
	// If only the left mouse button is being held down.
	else if(lmbDown)
	{
		// If the mouse is dragged across the X axis, move the camera left or right.
		x < lastX ? cam.pos.x -= 0.05 : cam.pos.x += 0.05;
		// Check to make sure the camera moves only in the specified boundaries.
		boundsCheckf(cam.pos.x, -2, 2, 0, 2);		
	}
	// If only the right mouse button is being held down.
	else if(rmbDown)
	{
		// If the mouse is dragged across the Y axis, move the camera up or down.
		y > lastY ? cam.pos.y -= 0.05 : cam.pos.y += 0.05;
		// Check to make sure the camera moves only in the specified boundaries.
		boundsCheckf(cam.pos.y, -2, 2, 0, 2);
	}
	// Reset the correct lighting to the proper position.
	// This is to stop the light moving with the camera,
	// when it's not supposed to.
	colouredLight ? setLight(&clockColouredLighting, GL_LIGHT0, light0_pos) :
			setLight(&clockWhiteLighting, GL_LIGHT0, light0_pos);
	// Set the last X and Y positions of the mouse to this variable.
	// This is to check to see which direction the mouse has moved the 
	// next time this function is called.
	lastX = x;
	lastY = y;
}

void mouseFunction(int b, int s, int x, int y)
{
	// If the left mouse button is active, toggle the state.
	if(b == GLUT_LEFT_BUTTON) s == GLUT_DOWN ? lmbDown = TRUE : lmbDown = FALSE;
	// If the right mouse button is active, toggle the state.
	if(b == GLUT_RIGHT_BUTTON) s == GLUT_DOWN ? rmbDown = TRUE : rmbDown = FALSE;
	// Set the last X and Y positions of the mouse to this variable.
	lastX = x;
	lastY = y;
}

void outputText(int x, int y, string t, BOOL size)
{
	// Set the 2d position to start drawing the text.
	glRasterPos2f(x, y); 
	// For each character in the string array.
	for (int i = 0; i < t.length(); i++) 
	{
		// If size is specified, draw a large font.
		if(!size) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, t[i]);
		// If size is unspecified, draw a smaller font.
		else glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, t[i]);
	}
}

void renderHUD(){
	// Set the matrix to Modelview, push it on to the stack and load it.
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();     
	glLoadIdentity();
	// Set the matrix to Projection to enable 2D viewing, 
	// push it on to the stack and load it.
	glMatrixMode (GL_PROJECTION);
	glPushMatrix();   
		glLoadIdentity();
		// Define the 2D window for the HUD.
		gluOrtho2D(0, 100, 0, 100); 
		// Output the current time to the HUD.
		outputText(2, 95, "Time: " + currentTime(), 0);
		// Create a string stream.
		stringstream ss;
		// Write the current speed level to the string stream.
		ss << "Speed: " << speedLevel;
		// Output the string stream to the HUD.
		outputText(82, 95, ss.str(), 0);

		// If the keyboard controls help dialog is toggled on ...
		if(showControls) 
		{
			// ... output the the keyboard controls help.
			outputText(2,10, "Keyboard Controls:",1);
			outputText(6,6, "Adjust Clock Speed (- / +)", 1);
			outputText(6,2, "Toggle Coloured Lighting - L", 1);
			outputText(50,2,"Cycle Colours - Up / Down Arrows",1);
			outputText(50,6,"Move Light - Left / Right Arrows",1);
		}
		// If the mouse controls help dialog is toggled on ... 
		else if(showMouseControls)
		{
			// ... output the mouse controls help.
			outputText(2,14, "Mouse Controls:",1);
			outputText(6,10,"Move Camera Left / Right - Left Mouse Button + Drag Left / Right",1);
			outputText(6,6,"Move Camera Up / Down - Right Mouse Button + Drag Up / Down",1);
			outputText(6,2,"Zoom In / Out - Left + Right Mouse Buttons + Drag Up / Down",1);
		}
		// If none of the help dialog is toggled on ...
		else 
		{
			// ... output the commands to show the help dialog.
			outputText(2,2, "Show Keyboard Controls - H", 1);
			outputText(60,2,"Show Mouse Controls - M",1);
		}
	// Pop the matrix and change the display method back to Modelview.
	glPopMatrix();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix();
}

void reshapeFunction(int w, int h)
{
	// If the window is resized, keep the proper aspect ratio.
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
	glMatrixMode (GL_MODELVIEW);
}

void setLight(lighting * light, int GL_LIGHT, float pos[]) 
{
	//Set the specified light to the specified light position and colour.
	glLightfv(GL_LIGHT, GL_AMBIENT, light->ambient);
	glLightfv(GL_LIGHT, GL_DIFFUSE, light->diffuse);
	glLightfv(GL_LIGHT, GL_SPECULAR,light->specular);
	glLightfv(GL_LIGHT, GL_POSITION, pos);
}

void updateSpeed()
{
	// Check the speed level is within the specified boundaries.
	boundsChecki(speedLevel, 1, 30, 0);
	// Set the speeds of each hand, to a default value multipled by the speed level.
	lHandRSpeed = 0.3 * speedLevel;
	sHandRSpeed = 0.024933 * speedLevel;
}