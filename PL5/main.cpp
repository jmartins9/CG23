#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ, inc = 0;


void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawTree(float x, float z) {
	glPushMatrix();

	glColor3f(0.1f, 0.0f, 0.0f);
	glTranslatef(x, 0, z);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(1, 6, 8, 4);
	glPopMatrix();
	glColor3f(0.0f, 0.1f, 0.0f);
	glTranslatef(0, 4, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(2, 8, 8, 4);

	glColor3f(1, 1, 1);
	glPopMatrix();
}

void drawTeapots1() {
	glPushMatrix();

	float angle = 2 * M_PI / 10;
	for (int i = 0; i < 10; i++) {
		glPushMatrix();
		glRotatef((i * angle + inc) * 180 / M_PI, 0, 1, 0);
		glTranslatef(10 , 2, 0);
		glutSolidTeapot(2);
		glPopMatrix();
	}

	glPopMatrix();
}

void drawTeapots2() {
	glPushMatrix();

	float angle = 2 * M_PI / 10;
	for (int i = 0; i < 10; i++) {
		glPushMatrix();
		glRotatef((i * angle - inc) * 180 / M_PI, 0, 1, 0);
		glTranslatef(30, 2, 0);
		glRotatef(90, 0, 1, 0);
		glutSolidTeapot(2);
		glPopMatrix();
	}

	glPopMatrix();
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();
	
	// put code to draw scene in here
	
	// Same random seed
	srand(1);

	// Draw trees
	float x, z;

	for (int i = 0; i < 200; i++) {
		x = ((float) rand() / RAND_MAX) * 200 - 100; // Number between -100 and 100
		z = ((float) rand() / RAND_MAX) * 200 - 100;

		if (x * x + z * z < 50 * 50) {
			i--;
		}
		else {
			drawTree(x, z);
		}
	}

	// Draw Torus
	glColor3f(1, 0, 1);
	glutSolidTorus(1, 2, 10, 10);
	glColor3f(1, 1, 1);
	
	// Draw Teapots1 
	glColor3f(0.0f, 0.0f, 1.0f);//Blue
	drawTeapots1();

	// Draw Teapots2
	glColor3f(1.0f, 0.5f, 0.0f);//Orange
	drawTeapots2();

	glColor3f(1, 1, 1);
	inc += 0.02;

	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
