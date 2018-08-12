// cg_hw4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stanford_bunny.h"
#include <GL/glut.h>

// window
int winWidh = 640;
int winHeight = 640;
//const int PERIOD = 30;

// lookat paremeters
GLfloat eye[3] = {0, 0, 10};
GLfloat ref[3] = {0, 0, 0};
GLfloat upv[3] = {0, 1, 0};
GLfloat dcamera = 1;

// objects
int tbGrids = 4;  // 4*4 bunnies, 4, 6, 8, ....
const GLfloat gridSize = 1.0;
int bunnyNum = 1;  // 1*1, 1, 3, 5, 7, ...

bool isNaive = true;  // naive or list
GLuint tableList;
GLuint bunnyList;
enum ObjType {TABLE, BUNNY};

// rotate
GLfloat rtTheta = 0;
GLfloat dtheta = 5;
bool isRotate = false;


void init() {
	GLfloat light0PosType[] = { 10, 10, 10, 1 };
	GLfloat gray[] = { 0.1, 0.4, 0.4, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gray);
	glLightfv(GL_LIGHT0, GL_POSITION, light0PosType);
	glLightfv(GL_LIGHT0, GL_AMBIENT, gray);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);

	glClearColor(0, 0.5, 0, 1);  // green background

	tableList = glGenLists(1);
	bunnyList = glGenLists(1);
}
void drawTable() {
	GLfloat tbWidth = tbGrids * gridSize;
	GLfloat tbHeight = 0.5 * gridSize;
	GLfloat ftWidth = 0.5 * gridSize;
	GLfloat ftHeight = 3 * gridSize;
	GLfloat ftMovexz = (tbGrids / 2 - 1) * gridSize;
	GLfloat ftMovey = (tbHeight + ftHeight) / 2;

	glPushMatrix();
	glScalef(tbWidth, tbHeight, tbWidth);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0 * ftMovexz, -1.0 * ftMovey, 1.0 * ftMovexz);
	glScalef(ftWidth, ftHeight, ftWidth);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0 * ftMovexz, -1.0 * ftMovey, -1.0 * ftMovexz);
	glScalef(ftWidth, ftHeight, ftWidth);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0 * ftMovexz, -1.0 * ftMovey, 1.0 * ftMovexz);
	glScalef(ftWidth, ftHeight, ftWidth);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0 * ftMovexz, -1.0 * ftMovey, -1.0 * ftMovexz);
	glScalef(ftWidth, ftHeight, ftWidth);
	glutSolidCube(1.0);
	glPopMatrix();
}
void drawBunny() {
	GLfloat bx, bz;
	GLfloat by = 0.7;
	GLfloat scale = 2.0;

	for (int i = 0; i < bunnyNum; i++) {
		for (int j = 0; j < bunnyNum; j++) {

			bx = i * gridSize - (bunnyNum - 1) / 2 * gridSize;
			bz = j * gridSize - (bunnyNum - 1) / 2 * gridSize;

			glPushMatrix();
			glTranslatef(bx, by, bz);
			glScalef(2, 2, 2);
			DrawBunny();
			glPopMatrix();
		}
	}
}
void drawObjectNaive() {
	drawTable();
	drawBunny();
}
void genList(ObjType type) {  // only called when object changes
	if (type == TABLE) {
		glNewList(tableList, GL_COMPILE);
		drawTable();
		glEndList();
	}
	else {
		glNewList(bunnyList, GL_COMPILE);
		drawBunny();
		glEndList();
	}
}
void drawObjectList() {
	glCallList(tableList);
	glCallList(bunnyList);
}
void getFPS()
{
	static int frame = 0, time, timebase = 0;
	static char buffer[256];

	char mode[64];
	if (!isNaive)
		strcpy(mode, "display list");
	else
		strcpy(mode, "naive");

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(buffer, "FPS:%4.2f %s",
			frame*1000.0 / (time - timebase), mode);
		timebase = time;
		frame = 0;
	}

	char *c;
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glOrtho(0, 480, 0, 480, -1, 1);    // 位置正投影
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glRasterPos2f(10, 10);
	for (c = buffer; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glEnable(GL_DEPTH_TEST);
}
void redrawf() {
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2],  ref[0], ref[1], ref[2],  upv[0], upv[1], upv[2]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);  // white pen

	GLfloat theta;
	if (isRotate) {
		rtTheta += dtheta;  // else do noth
		rtTheta = int(rtTheta) % 360;
	}
	glRotatef(rtTheta, 0, 1, 0);

	// draw teapot and table
	if (isNaive) {
		drawObjectNaive();
	}
	else {
		drawObjectList();
	}

	getFPS();
	glutSwapBuffers();
}
void reshapef(int newWidth, int newHeight) {
	glViewport(0, 0, newWidth, newHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float whRatio = (float)newWidth / (float)newHeight;
	gluPerspective(45, whRatio, 5, -100);
	
	glMatrixMode(GL_MODELVIEW);
}
void keyboardf(unsigned char k, int x, int y) {
	switch (k) {
	case 'w':
		eye[1] += dcamera;
		ref[1] += dcamera;
		break;
	case 's':
		eye[1] -= dcamera;
		ref[1] -= dcamera;
		break;
	case 'd':
		eye[0] += dcamera;
		ref[0] += dcamera;
		break;
	case 'a':
		eye[0] -= dcamera;
		ref[0] -= dcamera;
		break;
	case 'z':
		eye[2] -= dcamera;  // more near
		//ref[3] += dcamera;
		break;
	case 'c':
		eye[2] += dcamera;  // more far
		//ref[3] -= dcamera;
		break;

	case 'y':
		eye[1] += dcamera;
		break;
	case 'h':
		eye[1] -= dcamera;
		break;

	case 'l':
		if (isNaive) {
			genList(TABLE);
			genList(BUNNY);
		}
		isNaive = !isNaive;
		break;
		
	case 'i':
		if (bunnyNum + 1 == tbGrids) {
			tbGrids += 2;
			genList(TABLE);
		}
		bunnyNum += 2;
		genList(BUNNY);
		break;
	case 'k':
		if (bunnyNum > 1) {
			bunnyNum -= 2;
			genList(BUNNY);
		}
		break;

	case 'u':
		tbGrids += 2;
		genList(TABLE);
		break;
	case 'j':
		if (tbGrids > 4) {
			if (tbGrids - 1 == bunnyNum) {
				bunnyNum -= 2;
				genList(BUNNY);
			}
			tbGrids -= 2;
			genList(TABLE);
		}
		break;

	case ' ':
		isRotate = !isRotate;
		break;

	default:
		printf("unknown key %c\n", k);
		break;
	}
}
void idlef() {
	glutPostRedisplay();
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(200, 50);
	glutInitWindowSize(winWidh, winHeight);
	glutCreateWindow("Computer graphics homework4");

	init();
	glutDisplayFunc(redrawf);
	glutReshapeFunc(reshapef);
	glutKeyboardFunc(keyboardf);
	//glutTimerFunc(PERIOD, timerf, 1);
	glutIdleFunc(idlef);

	glutMainLoop();
}

