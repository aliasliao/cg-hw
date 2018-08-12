// cg_hw2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glut.h>
#include <cmath>

void redraw();
void reshape(int newWidth, int newHeight);
void idle();

class wcPt3D
{
public:
	GLfloat x, y, z;
	wcPt3D(GLint px, GLint py, GLint pz):
		x(GLfloat(px)), y(GLfloat(py)), z(GLfloat(pz)) {}
};

wcPt3D t[40] = {
	wcPt3D(0, 0, 0), wcPt3D(150, 0, 0), wcPt3D(150, 150, 0), wcPt3D(0, 150, 0),
	wcPt3D(0, 0, 30), wcPt3D(150, 0, 30), wcPt3D(150, 150, 30), wcPt3D(0, 150, 30),

	wcPt3D(20, 20, 30), wcPt3D(40, 20, 30), wcPt3D(40, 40, 30), wcPt3D(20, 40, 30),
	wcPt3D(20, 20, 120), wcPt3D(40, 20, 120), wcPt3D(40, 40, 120), wcPt3D(20, 40, 120),

	wcPt3D(110, 20, 30), wcPt3D(130, 20, 30), wcPt3D(130, 40, 30), wcPt3D(110, 40, 30),
	wcPt3D(110, 20, 120), wcPt3D(130, 20, 120), wcPt3D(130, 40, 120), wcPt3D(110, 40, 120),

	wcPt3D(110, 110, 30), wcPt3D(130, 110, 30), wcPt3D(130, 130, 30), wcPt3D(110, 130, 30),
	wcPt3D(110, 110, 120), wcPt3D(130, 110, 120), wcPt3D(130, 130, 120), wcPt3D(110, 130, 120),

	wcPt3D(20, 110, 30), wcPt3D(40, 110, 30), wcPt3D(40, 130, 30), wcPt3D(20, 130, 30),
	wcPt3D(20, 110, 120), wcPt3D(40, 110, 120), wcPt3D(40, 130, 120), wcPt3D(20, 130, 120),
};

wcPt3D c[4] = {
	wcPt3D(75, 75, 0), wcPt3D(120, 240, 0), wcPt3D(420, 240, 0), wcPt3D(690, 240, 0)
};

// 变换速度：
float dy = 10.0,		// 每帧在y轴方向移动5个单位
	  dt = 10.0,	// 每帧旋转10度
	  ds = 0.05;	// 每帧缩放5%
// 变换边界
const float my = 120.0,	// 上下120个单位
			mt = 360.0,	// 360度
			ms = 0.4;	// 最小为原来的40%
// 初始状态
float curry = 0.0,	// 初始y坐标偏移为0
	  currt = 0,	// 初始旋转了0度
      currs = 1.0;	// 初始没有缩放

void updateCurr()
{
	if (curry > -my && curry < my)
		curry += dy;
	else
	{
		dy = -dy;
		curry += dy;
	}

	if (currt < mt)
		currt += dt;
	else
		currt = 0;

	if (currs > ms && currs <= 1.0)
		currs -= ds;
	else
	{
		ds = -ds;
		currs -= ds;
	}
}

GLfloat v[3];
GLfloat* get3V(int n)
{
	v[0] = t[n].x;
	v[1] = t[n].y;
	v[2] = t[n].z;

	return v;
}
void drawTable()
{
	for (int i = 0; i < 5; i++)
	{
		glBegin(GL_QUADS);
			glVertex3fv(get3V(0 + i*8));
			glVertex3fv(get3V(1 + i*8));
			glVertex3fv(get3V(2 + i*8));
			glVertex3fv(get3V(3 + i*8));
		glEnd();
	
		glBegin(GL_QUADS);
			glVertex3fv(get3V(4 + i*8));
			glVertex3fv(get3V(5 + i*8));
			glVertex3fv(get3V(6 + i*8));
			glVertex3fv(get3V(7 + i*8));
		glEnd();
	
		glBegin(GL_QUADS);
			glVertex3fv(get3V(0 + i*8));
			glVertex3fv(get3V(1 + i*8));
			glVertex3fv(get3V(5 + i*8));
			glVertex3fv(get3V(4 + i*8));
		glEnd();
	
		glBegin(GL_QUADS);
			glVertex3fv(get3V(1 + i*8));
			glVertex3fv(get3V(2 + i*8));
			glVertex3fv(get3V(6 + i*8));
			glVertex3fv(get3V(5 + i*8));
		glEnd();
	
		glBegin(GL_QUADS);
			glVertex3fv(get3V(2 + i*8));
			glVertex3fv(get3V(3 + i*8));
			glVertex3fv(get3V(7 + i*8));
			glVertex3fv(get3V(6 + i*8));
		glEnd();
	
		glBegin(GL_QUADS);
			glVertex3fv(get3V(3 + i*8));
			glVertex3fv(get3V(0 + i*8));
			glVertex3fv(get3V(4 + i*8));
			glVertex3fv(get3V(7 + i*8));
		glEnd();
	}
}

void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	updateCurr();
	
	glMatrixMode(GL_MODELVIEW);

	// table 1
	glLoadIdentity();
	glTranslatef(0.0, curry, 0.0);
	glTranslatef(c[1].x-c[0].x, c[1].y-c[0].y, c[1].z-c[0].z);
	glTranslatef(75.0, 0.0, 0.0);
	glRotatef(30.0, 0.0, 1.0, 0.0);
	glTranslatef(-75.0, 0.0, 0.0);
	drawTable();

	//table 2
	glLoadIdentity();
	glTranslatef(c[2].x-c[0].x, c[2].y-c[0].y, c[2].z-c[0].z);
	glTranslatef(75.0, 0.0, 0.0);
	glRotatef(currt, 0.0, 1.0, 0.0);
	glTranslatef(-75.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -120.0);
	drawTable();

	//table 3
	glLoadIdentity();
	glTranslatef(c[3].x-c[0].x, c[3].y-c[0].y, c[3].z-c[0].z);
	glTranslatef(75.0, 75.0, 0.0);
	glRotatef(-30.0, 0.0, 1.0, 0.0);
	glScalef(currs, currs, currs);
	glTranslatef(-75.0, -75.0, 0.0);
	drawTable();

	glutSwapBuffers();
}

void onTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(30, onTimer, 1);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(810, 480);
	glutCreateWindow("Computer graphics homework2");
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glutDisplayFunc(redraw);
	glutTimerFunc(30, onTimer, 1);

	glMatrixMode(GL_PROJECTION);

	//gluLookAt(0.0, 0.0, 0.0,   0.0, 0.0, -1.0,   0.0, 1.0, 0.0);	// 正视
	gluLookAt(0.0, 0.0, 0.0,   0.0, -1.0, -4.0,   0.0, 4.0, -1.0);	// 斜俯视
	//gluLookAt(0.0, 0.0, 0.0,   0.0, -1.0, 0.0,   0.0, 0.0, -1.0);	// 俯视
	glOrtho(0.0, 810.0, 0.0, 480.0, 200.0, -200.0);

	//gluLookAt(405.0, 240.0, 300.0,   405.0, 240.0, 0.0,   0.0, 1.0, 0.0);
	////gluPerspective(30, 810/480, 200, 500);
	//glFrustum(-135, 135, -80, 80, 100, 500);

	glutMainLoop();

    return 0;
}

