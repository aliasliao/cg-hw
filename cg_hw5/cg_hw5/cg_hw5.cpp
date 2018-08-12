// cg_hw5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glut.h>

int winWidth = 640;
int winHeight = 640;
const int period = 30;

int eye[3] = { 0, 0, 5 };
int ref[3] = { 0, 0, 0 };
int upv[3] = { 0, 1, 0 };
int deye = 1;

// color
float red[4] = { 1, 0, 0, 1 };
float green[4] = { 0 ,1 ,0, 1 };
float blue[4] = { 0, 0, 1, 1 };
float yellow[4] = { 1, 1, 0, 1 };
float cyan[4] = { 0, 1, 1, 1 };
float gold[4] = { 1, 0.843, 0, 1 };
float gray[4] = { 0.5, 0.5, 0.5, 1 };
float white[4] = { 1, 1, 1, 1 };
float black[4] = { 0, 0, 0, 1 };
float pink[4] = { 1, 0.753, 0.796, 1 };
float other[4] = { 1, 0.271, 0, 1 };

float lightPos[2][4] = { { 10, 10, 10, 1 }, { 0, 3, 0, 1 } };
//float l0Pos[4] = { 3, 3, 3, 1 };
//float l1Pos[4] = { 0, 5, 0, 1 };
float l1Vec[3] = { 0, -1, 0 };
float l1Ang = 30;
const float l1Exp = 2.5;

float dlight = 0.5;
int lightNo = 0;

float spTheta = 0;
float dtheta = 5;

bool isFill = true;
bool isPers = false;
bool isSpin = false;
bool isWhiteLight = true;

void activeL0() {
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos[0]);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, gray);
	if (isWhiteLight) {
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
		glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	}
	else {
		glLightfv(GL_LIGHT0, GL_DIFFUSE, other);
		glLightfv(GL_LIGHT0, GL_SPECULAR, other);
	}
	glDisable(GL_LIGHT1);
	glEnable(GL_LIGHT0);
	lightNo = 0;
}
void activeL1() {
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos[1]);
	//glLightfv(GL_LIGHT1, GL_AMBIENT, gray);
	if (isWhiteLight) {
		glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
		glLightfv(GL_LIGHT1, GL_SPECULAR, white);
	}
	else {
		glLightfv(GL_LIGHT1, GL_DIFFUSE, other);
		glLightfv(GL_LIGHT1, GL_SPECULAR, other);
	}
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l1Vec);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, l1Ang);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, l1Exp);
	glDisable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	lightNo = 1;
}
void activeLight(int num) {
	if (num == 0) {
		activeL0();
	}
	else {
		activeL1();
	}
}
void init() {
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gray);

	glEnable(GL_LIGHTING);
	activeLight(0);

	glEnable(GL_DEPTH_TEST);

	glClearColor(0, 0, 0, 1);
}
void drawObject() {
	float tb[3] = { 2, 0.4, 2 };
	float ft[3] = { 0.3, 1.5, 0.3 };
	float ftBias[3] = { 0.5, (tb[1] + ft[1]) / 2, 0.5 };

	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 25);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
	glPushMatrix();
	glScalef(tb[0], tb[1], tb[2]);
	glutSolidCube(1);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
	glPushMatrix();
	glTranslatef(ftBias[0], -1 * ftBias[1], ftBias[2]);
	glScalef(ft[0], ft[1], ft[2]);
	glutSolidCube(1);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, yellow);
	glPushMatrix();
	glTranslatef(ftBias[0], -1 * ftBias[1], -1 * ftBias[2]);
	glScalef(ft[0], ft[1], ft[2]);
	glutSolidCube(1);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cyan);
	glPushMatrix();
	glTranslatef(-1 * ftBias[0], -1 * ftBias[1], ftBias[2]);
	glScalef(ft[0], ft[1], ft[2]);
	glutSolidCube(1);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
	glPushMatrix();
	glTranslatef(-1 * ftBias[0], -1 * ftBias[1], -1 * ftBias[2]);
	glScalef(ft[0], ft[1], ft[2]);
	glutSolidCube(1);
	glPopMatrix();

	// teapot
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gold);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);
	glPushMatrix();
	glTranslatef(0, 0.55, 0);
	glScalef(0.5, 0.5, 0.5);
	glutSolidTeapot(1);
	glPopMatrix();

	// reset these to default for table
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
}
void redrawf() {
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2], ref[0], ref[1], ref[2], upv[0], upv[1], upv[2]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);  // white pen

	if (isFill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLfloat theta;
	if (isSpin) {
		spTheta += dtheta;  // else do noth
		spTheta = int(spTheta) % 360;
	}
	glRotatef(spTheta, 0, 1, 0);

	drawObject();

	glutSwapBuffers();
};
void switchProject(float aspect) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (isPers) {
		gluPerspective(45, aspect, 3, -100);
	}
	else {
		glOrtho(-3, 3, -3, 3, -10, 10);
	}
	glMatrixMode(GL_MODELVIEW);
}
void reshapef(int newWidth, int newHeight) {
	winWidth = newWidth;
	winHeight = newHeight;

	glViewport(0, 0, newWidth, newHeight);

	float whRatio = (float)newWidth / (float)newHeight;
	switchProject(whRatio);

	glMatrixMode(GL_MODELVIEW);
}
void keyboardf(unsigned char k, int x, int y) {
	switch (k)
	{
	case 'w':
		eye[1] += deye;
		ref[1] += deye;
		break;
	case 's':
		eye[1] -= deye;
		ref[1] -= deye;
		break;
	case 'a':
		eye[0] -= deye;
		ref[0] -= deye;
		break;
	case 'd':
		eye[0] += deye;
		ref[0] += deye;
		break;
	case 'z':
		eye[2] -= deye;
		ref[2] -= deye;
		break;
	case 'c':
		eye[2] += deye;
		ref[2] += deye;
		break;

	case '=':
		eye[1] += deye;
		break;
	case '-':
		eye[1] -= deye;
		break;

	case 'i':
		lightPos[lightNo][1] += dlight;
		activeLight(lightNo);
		break;
	case 'k':
		lightPos[lightNo][1] -= dlight;
		activeLight(lightNo);
		break;
	case 'j':
		lightPos[lightNo][0] -= dlight;
		activeLight(lightNo);
		break;
	case 'l':
		lightPos[lightNo][0] += dlight;
		activeLight(lightNo);
		break;
	case 'm':
		lightPos[lightNo][2] += dlight;
		activeLight(lightNo);
		break;
	case ',':
		lightPos[lightNo][2] -= dlight;
		activeLight(lightNo);
		break;

	case '1':  // switch light color
		isWhiteLight = !isWhiteLight;
		activeLight(lightNo);
		break;
	case '2':  // switch light
		lightNo = (lightNo + 1) % 2;
		activeLight(lightNo);
		break;

	case 'o':
		isFill = !isFill;
		break;
	case 'p':
		isPers = !isPers;
		switchProject((float)winWidth / (float)winHeight);
		break;
	case ' ':
		isSpin = !isSpin;
		break;

	default:
		printf("unknown key %c\n", k);
		break;
	}
}
void timerf(int value) {
	glutPostRedisplay();
	glutTimerFunc(period, timerf, 1);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(200, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Computer graphics homework4");

	init();
	glutDisplayFunc(redrawf);
	glutReshapeFunc(reshapef);
	glutKeyboardFunc(keyboardf);
	glutTimerFunc(period, timerf, 1);

	glutMainLoop();
}

