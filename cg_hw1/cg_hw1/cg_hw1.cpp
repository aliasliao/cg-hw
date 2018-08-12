// cg_hw1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include <GL/glut.h>


void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 700.0, 0.0, 500.0);
}

void drawFlag()
{
	float bigStar[10][2] = {
		{50+83.832, 50+400-84.000},
		{50+105.166, 50+400-83.833},
		{50+111.499, 50+400-105.166},
		{50+94.499, 50+400-118.166},
		{50+77.499, 50+400-105.166},

		{50+94.500, 50+400-49.833},
		{50+138.833, 50+400-84.000},
		{50+121.333, 50+400-138.333},
		{50+67.332, 50+400-138.665},
		{50+50.000, 50+400-84.000}
	};

	float centerBig[2] = {50+100.000, 50+400-100.000};
	float centerSmall[4][2] = {
		{50+200.000, 50+400-40.000}, 
		{50+240.000, 50+400-80.000}, 
		{50+240.000, 50+400-140.000}, 
		{50+200.000, 50+400-180.000}, 
	};

	float smallStar[4][10][2];

	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < 10; i++)
		{
			float oldX = bigStar[i][0];
			float oldY = bigStar[i][1];
			float newX = (oldX - centerBig[0]) / 3;
			float newY = (oldY - centerBig[1]) / 3;
			double theta;
			switch (k)
			{
			case 0:
				theta = 3.14 / 6;
				break;
			case 1:
				theta = 3.14 / 3;
				break;
			case 2:
				theta = 0;
				break;
			case 3:
				theta = 3.14 / 6;
				break;
			default:
				break;
			}
			//double cx = newX * cos(theta) + newY * sin(theta); float cs0 = centerSmall[k][0];
			//double sx = newY * cos(theta) - newX * sin(theta); float cs1 = centerSmall[k][1];
			smallStar[k][i][0] = newX * cos(theta) + newY * sin(theta) + centerSmall[k][0];
			smallStar[k][i][1] = newY * cos(theta) - newX * sin(theta) + centerSmall[k][1];
		}
	}

	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex2i(50, 50);
	glVertex2i(650, 50);
	glVertex2i(650, 450);
	glVertex2i(50, 450);
	glEnd();

	glColor3f(1.0, 1.0, 0.0);

	glBegin(GL_POLYGON);
	for (int i = 0; i < 5; i++)
	{
		glVertex2fv(bigStar[i]);
	}
	glEnd();

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 5; i++)
	{
		glVertex2fv(bigStar[i]);
		glVertex2fv(bigStar[(i+1)>=5 ? (i+1-5) : (i+1)]);
		glVertex2fv(bigStar[i+5]);
	}
	glEnd();

	for (int k = 0; k < 4; k++)
	{
		glBegin(GL_POLYGON);
		for (int i = 0; i < 5; i++)
		{
			glVertex2fv(smallStar[k][i]);
		}
		glEnd();

		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 5; i++)
		{
			glVertex2fv(smallStar[k][i]);
			glVertex2fv(smallStar[k][(i + 1) >= 5 ? (i + 1 - 5) : (i + 1)]);
			glVertex2fv(smallStar[k][i + 5]);
		}
		glEnd();
	}

	glFlush();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(700, 500);
	glutCreateWindow("Computer Graphics homework1");

	init();
	glutDisplayFunc(drawFlag);
	glutMainLoop();

    return 0;
}
