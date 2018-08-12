// cg_hw3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glut.h>

const int PERIOD = 30,	// render period
	WIN_WIDTH = 640,	// initial window width
	WIN_HEIGHT = 640;   // initial window height
GLfloat x0 = 0, y0 = 0, z0 = 300,		// lookat, position of eye
	xref = 0, yref = 0, zref = 0,	// position of reference point
	xv = 0, yv = 1, zv = 0;		// up vector
GLfloat dcamera = 10;	// camera move speed
bool isProject = true,
	isFill = true,
	isRotate = false;
GLfloat tx = 0, tz = 0;	// teapot's location on table
const int dtp = 5;  // teapot move speed
GLfloat rotateTheta = 0, dtheta = 5;  // rotate count and rotate speed
GLfloat nWidth = 0, nHeight = 0;  // store window size
	
void init()
{
	//--
	//GLfloat light0PosType[] = { 50, 50, 50, 1 };	// far light
	GLfloat light0PosType[] = { 200, 200, 200, 1 };	// far light
	GLfloat blackColor[] = {0, 0, 0, 1};
	GLfloat whiteColor[] = {1, 1, 1, 1};
	GLfloat grayColor[] = {0.9, 0.9, 0.9, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, light0PosType);	// set light pos
	glLightfv(GL_LIGHT0, GL_AMBIENT, whiteColor);	// set env light color
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteColor);	// set man light color
	//glLightfv(GL_LIGHT0, GL_SPECULAR, whiteColor);	// set high light color
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	//--

	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);	// window background color is black
}

void drawObject()
{
	const GLfloat btWidth = 80.0, btSy = 0.1;
	const GLfloat stWidth = 10.0, stSy = 6.0;
	const GLfloat tpSize = 20;
	GLfloat stMx, stMy, stMz;
	stMx = stMz = btWidth / 4;
	stMy = (btWidth*btSy + stWidth*stSy) / 2.0;

	glPushMatrix();
	glScalef(1, btSy, 1);	// 80*8*80
	glutSolidCube(btWidth);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0 * stMx, -1.0 * stMy, 1.0 * stMz);
	glScalef(1, stSy, 1);
	glutSolidCube(stWidth);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0 * stMx, -1.0 * stMy, 1.0 * stMz);
	glScalef(1, stSy, 1);
	glutSolidCube(stWidth);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0 * stMx, -1.0 * stMy, -1.0 * stMz);
	glScalef(1, stSy, 1);
	glutSolidCube(stWidth);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0 * stMx, -1.0 * stMy, -1.0 * stMz);
	glScalef(1, stSy, 1);
	glutSolidCube(stWidth);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(tx, 18, tz);
	glutSolidTeapot(tpSize);
	glPopMatrix();
}
void redrawf()
{
	glLoadIdentity();
	//gluLookAt(0, 0, 0,   0, -1, -2,   0, 1, 0);	// ^ !important: lookat - projection - ortho
	gluLookAt(x0, y0, z0,  xref, yref, zref, xv, yv, zv);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1, 1, 1);	// white pen
	//isFill = false;
	if (isFill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLfloat theta;
	if (isRotate) {
		rotateTheta += dtheta;	// else do noth
		rotateTheta = int(rotateTheta) % 360;
	}
	glRotatef(rotateTheta, 0, 1, 0);

	drawObject();	// draw teapot and table

	//glutSwapBuffers();
	glFlush();
}

void reshapef(GLint newWidth, GLint newHeight)
{
	nWidth = newWidth;
	nHeight = newHeight;

	glViewport(0, 0, newWidth, newHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	// !!!?
	if (isProject)
		glOrtho(-100, 100, -100, 100, -100, 500);	// fit this rectangle to viewport
	else
		gluPerspective(30, 1, 100, -100);
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void keyf(unsigned char k, int x, int y)
{
	switch (k)
	{
	case '8':	// up
		y0 -= dcamera;
		break;
	case '2':
		y0 += dcamera;
		break;
	case '4':
		x0 += dcamera;
		break;
	case '6':
		x0 -= dcamera;
		break;
	case '1':
		z0 -= dcamera;
		break;
	case '3':
		z0 += dcamera;
		break;

	case 'w':
		y0 += dcamera;
		yref += dcamera;
		break;
	case 's':
		y0 -= dcamera;
		yref -= dcamera;
		break;
	case 'a':
		x0 -= dcamera;
		xref -= dcamera;
		break;
	case 'd':
		x0 += dcamera;
		xref += dcamera;
		break;
	case 'z':
		z0 += dcamera;
		zref += dcamera;
		break;
	case 'c':
		z0 -= dcamera;
		zref -= dcamera;
		break;

	case 'p':
		isProject = !isProject;
		reshapef(nWidth, nHeight);
		break;

	case 'o':
		isFill = !isFill;
		break;

	case ' ':
		isRotate = !isRotate;
		break;

	case 'i':
		tz -= dtp;
		break;
	case 'k':
		tz += dtp;
		break;
	case 'j':
		tx -= dtp;
		break;
	case 'l':
		tx += dtp;
		break;

	default:
		break;
	}
}

void timerf(int value)
{
	glutPostRedisplay();
	glutTimerFunc(PERIOD, timerf, 1);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowPosition(200, 50);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("Computer Graphics Homework3");

	init();
	glutDisplayFunc(redrawf);
	glutReshapeFunc(reshapef);
	glutKeyboardFunc(keyf);
	glutTimerFunc(PERIOD, timerf, 1);

	glutMainLoop();
}

