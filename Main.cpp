#include <Windows.h>
#include "iostream"
#include <glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "CameraSpectator.h"
#include "Tools.h"
#include "vector"
#include "Map.h"
#include "Building.h"
#include "Ball.h"
#include "Card.h"
#include "SkyCube.h"

#define SPEED 0.3
#define ROTATION 3

CCamera cam;
SkyCube skyCube;
Map brasovMap;
Ball *ball;
bool up,down,left,right,rotLeft,rotRight, jump;
int texNr=0;
Card card;
int lastCheckPointKey;

void initGL() 
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
	try
	{
		
		brasovMap = Map("Map.xml");
		ball = new Ball(WIDTH / 8, Point(0, 0, 0));
		Point checkPointPosition = brasovMap.GetPoint(CHECKPOINT).getCenter();
		card = Card(Point(checkPointPosition.x, checkPointPosition.y + 0.5, checkPointPosition.z));
		lastCheckPointKey = STARTPOINT;
	}
	catch(char* message)
	{
		throw message;
	}
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); 
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
   
   glLoadIdentity();                 // Reset the model-view matrix

   glTranslatef(0.0f, -1.0f, -10.0f); 
   glRotatef(5.0,1,0,0);

   ball->SetTexNr(texNr);
   ball->Draw();
   cam.Render();
   skyCube.Draw();

   brasovMap.Draw();
   card.Draw();

	Building build1=Building(Point(1.35,0.,1.3),8,2);
	build1.Draw();	
	Building build2=Building(Point(5.2,0.,2.2),8,2);
	build2.Draw();
	Building build3=Building(Point(20.2,0.,18.2),20,3);
	build3.Draw();

    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}
void timer(int value) 
{
	glutPostRedisplay();
	glutTimerFunc(15, timer, 0);
	SF3dVector center = cam.GetPosition();
	//Point center = ball->GetPosition();
	if (jump)
	{
		ball->Jump(jump);
	}
	if (left)
	{
		cam.MoveX(-SPEED); 
		ball->MoveX(-SPEED);
		center = cam.GetPosition();
		if (brasovMap.BallCollision(lastCheckPointKey, Point(center.x, center.y, center.z)) == BallStreetPosition::Outside)
		{
			cam.MoveX(SPEED); 
			ball->MoveX(SPEED);
		}		
	}
	if (right)
	{
		cam.MoveX(SPEED); 
		ball->MoveX(SPEED);
		center = cam.GetPosition();
		if (brasovMap.BallCollision(lastCheckPointKey, Point(center.x, center.y, center.z)) == BallStreetPosition::Outside)
		{
			cam.MoveX(-SPEED);
			ball->MoveX(-SPEED);
		}
	}
	if (up)
	{
		cam.MoveZ(-SPEED); 
		ball->MoveZ(SPEED);
		center = cam.GetPosition();
		if (brasovMap.BallCollision(lastCheckPointKey, Point(center.x, center.y, center.z)) == BallStreetPosition::Outside)
		{
			cam.MoveZ(SPEED);
			ball->MoveZ(-SPEED);
		}
	}
	if (down)
	{
		cam.MoveZ(SPEED); 
		ball->MoveZ(-SPEED);
		center = cam.GetPosition();
		if (brasovMap.BallCollision(lastCheckPointKey, Point(center.x, center.y, center.z)) == BallStreetPosition::Outside)
		{
			cam.MoveZ(-SPEED);
			ball->MoveZ(SPEED);
		}
	}
	if(rotLeft)
	{
		cam.RotateY(ROTATION);
	}
	if(rotRight)
	{
		cam.RotateY(-ROTATION);
	}
	skyCube.SetPoz(Point(cam.GetPosition().x,0,cam.GetPosition().z));
}

void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
	// Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);

	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 300.0f);
}

void keyboardPressed (unsigned char key, int x, int y)
{
	switch (key) 
	{
		//  case 'w':cam.RotateX(-5);break;
		//  case 's':cam.RotateX(5);break;
	case 'a':rotLeft=true; break;
	case 'd':rotRight=true; break;
	//case 'w':cam.RotateX(5);break;
	//case 's':cam.RotateX(-5);break;
	case ' ':jump=true; break;
	case 't':if(texNr==4)
				 texNr=0;
			 else
			     texNr++;
			 break;
	}
}
void keyboardReleased (unsigned char key, int x, int y)
{
	switch (key) 
	{
	case 'a':rotLeft=false; break;
	case 'd':rotRight=false; break;
	}
}

void handleSpecialKeyPressed(int key, int x, int y)
{
	switch (key) 
	{
	case GLUT_KEY_LEFT:left = true; break;
	case GLUT_KEY_RIGHT:right = true; break;
	case GLUT_KEY_UP:up = true; break;
	case GLUT_KEY_DOWN:down = true; break;
	}
}
void handleSpecialKeyReleased(int key, int x, int y) 
{
	switch (key) 
	{
	case GLUT_KEY_LEFT:left = false; break;
	case GLUT_KEY_RIGHT:right = false; break;
	case GLUT_KEY_UP:up = false; break;
	case GLUT_KEY_DOWN:down = false; break;
	}
}
int main(int argc, char** argv) 
{
	try
	{
		glutInit(&argc, argv);            // Initialize GLUT
		glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
		glutInitWindowSize(640, 480);   // Set the window's initial width & height
		glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
		glutCreateWindow("Test");          // Create window with the given title
		glutDisplayFunc(display);       // Register callback handler for window re-paint event
		glutReshapeFunc(reshape);       // Register callback handler for window re-size event
		initGL();                       // Our own OpenGL initialization
		glutKeyboardFunc(keyboardPressed);
   		glutKeyboardUpFunc(keyboardReleased);
  		glutSpecialFunc(handleSpecialKeyPressed);
   		glutSpecialUpFunc(handleSpecialKeyReleased);
		glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
		glutMainLoop();                 // Enter the infinite event-processing loop
	}
	catch(char* message)
	{
		std::cout<<message<<"\n";
	}
	catch(...)
	{
		return -5;
	}
	return 0;
}