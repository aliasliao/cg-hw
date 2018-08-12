// cg_hw6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <stdlib.h>
#include <iostream>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glext.lib")


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

float lightPos[4] = { 10, 10, 10, 1 };

float dlight = 0.5;

float spTheta = 0;  // spin theta
float dtheta = 5;

bool isFill = true;
bool isPers = true;
bool isSpin = false;
bool customTex = false;  // if teapot texture is custom
bool isMix = false;

GLuint texArray[4];
GLuint shaderProgram;

const GLchar *vertexSource = R"glsl(
	#version 400 core
	in vec3 position;
	in vec2 texcoord;

	out vec2 Texcoord;

	void main() {
		Texcoord = texcoord;
		gl_Position = vec4(position, 1.0);
	}
)glsl";
const GLchar *fragmentSource = R"glsl(
#version 400 core
	in vec2 Texcoord;

	out vec4 outColor;

	uniform sampler2D crack;
	uniform sampler2D spot;

	void main() {
		vec4 colCrack = texture(crack, Texcoord);
		vec4 colSpot = texture(spot, Texcoord);
		outColor = mix(colCrack, colSpot, 0.5);
	}
)glsl";

void updateL0() {
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}
GLuint genTexture() {
	const int width = 10;
	GLubyte texData[width][width][4];
	
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			if ((i + j) % 2 == 0) {
				texData[i][j][0] = 255;
				texData[i][j][1] = 0;
				texData[i][j][2] = 0;
				texData[i][j][3] = 255;
			}
			else {
				texData[i][j][0] = 0;
				texData[i][j][1] = 0;
				texData[i][j][2] = 0;
				texData[i][j][3] = 255;
			}
		}
	}

	GLuint texTure;
	glGenTextures(1, &texTure);
	glBindTexture(GL_TEXTURE_2D, texTure);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, width, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

	return texTure;
}
GLuint loadTexture(const char* fileName, const int width) {
	int length = width * width * 3;
	FILE* fp = fopen(fileName, "rb");
	unsigned char* data = (unsigned char*)malloc(length);

	fread(data, length, 1, fp);
	fclose(fp);

	for (int i = 0; i < width * width; i++) {
		int index = i * 3;

		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;
	}

	GLuint texTure;
	glGenTextures(1, &texTure);
	glBindTexture(GL_TEXTURE_2D, texTure);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, width, GL_RGB, GL_UNSIGNED_BYTE, data);

	free(data);
	return texTure;
}
void init() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	updateL0();
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	glClearColor(0, 0, 0, 1);

	///////////////////////
	glewExperimental = GL_TRUE;
	glewInit();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);

	GLint length;
	GLsizei num;
	char *log;
	std::cout << "*****log*****" << std::endl;
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
	if (length > 0) {
		log = (char *)malloc(length * sizeof(char));
		glGetShaderInfoLog(vertexShader, length, &num, log);
		fprintf(stderr, "%s\n", log);
		free(log);
	}

	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);
	if (length > 0) {
		log = (char *)malloc(length * sizeof(char));
		glGetShaderInfoLog(fragmentShader, length, &num, log);
		fprintf(stderr, "%s\n", log);
		free(log);
	}

	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
	if (length > 0) {
		log = (char *)malloc(length * sizeof(char));
		glGetProgramInfoLog(shaderProgram, length, &num, log);
		fprintf(stderr, "%s\n", log);
		free(log);
	}
	std::cout << "*****log*****" << std::endl;

	//glUseProgram(shaderProgram);
	///////////////////////

	texArray[0] = loadTexture("resource/Crack.bmp", 128);
	texArray[1] = loadTexture("resource/Monet.bmp", 512);
	texArray[2] = loadTexture("resource/Spot.bmp", 128);
	texArray[3] = genTexture();

	//////////////////////////
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, texArray[0]);

	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, texArray[2]);

	//glUniform1i(glGetUniformLocation(shaderProgram, "crack"), 1);
	//glUniform1i(glGetUniformLocation(shaderProgram, "spot"), 2);
	//////////////////////////
}
void texCube() {
	if (isMix) {
		//glUseProgram(shaderProgram);
	}
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texArray[0]);

	glBegin(GL_QUADS);
	glTexCoord2i(0.0, 0.0); glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2i(1.0, 0.0); glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2i(1.0, 1.0); glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2i(0.0, 1.0); glVertex3f(-0.5, 0.5, 0.5);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2i(0.0, 0.0); glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2i(1.0, 0.0); glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2i(1.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2i(0.0, 1.0); glVertex3f(-0.5, -0.5, 0.5);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2i(0.0, 0.0); glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2i(1.0, 0.0); glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2i(1.0, 1.0); glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2i(0.0, 1.0); glVertex3f(-0.5, 0.5, 0.5);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2i(0.0, 0.0); glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2i(1.0, 0.0); glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2i(1.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2i(0.0, 1.0); glVertex3f(-0.5, 0.5, -0.5);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2i(0.0, 0.0); glVertex3f(0.5, 0.5, 0.5);
	glTexCoord2i(1.0, 0.0); glVertex3f(0.5, 0.5, -0.5);
	glTexCoord2i(1.0, 1.0); glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2i(0.0, 1.0); glVertex3f(0.5, -0.5, 0.5);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2i(0.0, 0.0); glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2i(1.0, 0.0); glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2i(1.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2i(0.0, 1.0); glVertex3f(-0.5, -0.5, 0.5);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}
void drawObject() {
	float tb[3] = { 2, 0.4, 2 };
	float ft[3] = { 0.3, 1.5, 0.3 };
	float ftBias[3] = { 0.5, (tb[1] + ft[1]) / 2, 0.5 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
	glPushMatrix();
	glScalef(tb[0], tb[1], tb[2]);
	texCube();
	glPopMatrix();

	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
	glPushMatrix();
	glTranslatef(ftBias[0], -1 * ftBias[1], ftBias[2]);
	glScalef(ft[0], ft[1], ft[2]);
	texCube();
	glPopMatrix();

	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, yellow);
	glPushMatrix();
	glTranslatef(ftBias[0], -1 * ftBias[1], -1 * ftBias[2]);
	glScalef(ft[0], ft[1], ft[2]);
	texCube();
	glPopMatrix();

	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cyan);
	glPushMatrix();
	glTranslatef(-1 * ftBias[0], -1 * ftBias[1], ftBias[2]);
	glScalef(ft[0], ft[1], ft[2]);
	texCube();
	glPopMatrix();

	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
	glPushMatrix();
	glTranslatef(-1 * ftBias[0], -1 * ftBias[1], -1 * ftBias[2]);
	glScalef(ft[0], ft[1], ft[2]);
	texCube();
	glPopMatrix();

	// teapot
	GLuint teaTex = customTex ? texArray[3] : texArray[1];

	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gold);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);
	glPushMatrix();
	glTranslatef(0, 0.55, 0);
	glScalef(0.5, 0.5, 0.5);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, teaTex);
			glutSolidTeapot(1);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// reset these to default for table
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
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

	case 'm':
		lightPos[1] += dlight;
		updateL0();
		break;
	case ',':
		lightPos[1] -= dlight;
		updateL0();
		break;
	case 'j':
		lightPos[0] -= dlight;
		updateL0();
		break;
	case 'l':
		lightPos[0] += dlight;
		updateL0();
		break;
	case 'i':
		lightPos[2] += dlight;
		updateL0();
		break;
	case 'k':
		lightPos[2] -= dlight;
		updateL0();
		break;

	case ';':
		customTex = !customTex;
		break;
	case '/':
		isMix = !isMix;
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