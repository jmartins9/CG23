

#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <iostream>

using namespace std;

int startX, startY, tracking = 0;

float k = 2;
float alpha = 0;
float camX = 1, camY, camZ = 1;
float lx, ly, lz;
float dx, dy, dz;



int beta = 45, r = 50;
unsigned int t, tw, th;
unsigned char* imageData;
float  inc = 0;
vector<float> vertexB;
GLuint buffers[1];

void aux() {
    dx = lx - camX;
    dy = 0;
    dz = lz - camZ;

    camX = camX + k * dx;
    camY = camY + k * dy;
    camZ = camZ + k * dz;

    lx = lx + k * dx;
    ly = ly + k * dy;
    lz = lz + k * dz;
}

void aux1() {
    lx = camX + sin(alpha);
    ly = camY;
    lz = camZ + cos(alpha);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

float getHeight(int i, int j) {
    return imageData[i * tw + j];
}

float getHeightf(float x , float z) {
    float x1 = floor(x);
    float x2 = x1 + 1;
    float z1 = floor(z);
    float z2 = z1 + 1;

    float fz = z - z1;
    float fx = x - x1;

    float h_x1_z = getHeight(x1 + 128,z1 + 128) * (1-fz) + getHeight(x1 + 128,z2 + 128) * fz;
    float h_x2_z = getHeight(x2 + 128,z1 + 128) * (1-fz) + getHeight(x2 + 128,z2 + 128) * fz;

    return  h_x1_z * (1 - fx) + h_x2_z * fx;
}

void produtoExterno() {
    float rx = - dz;
    float ry = 0;
    float rz = dx - dy;
    camX = camX + k * rx;
    camY = camY + k * ry;
    camZ = camZ + k * rz;

    lx = lx + k * rx;
    ly = ly + k * ry;
    lz = lz + k * rz;
}


void drawTree(float x, float z) {
    glPushMatrix();

    glColor3f(0.1f, 0.0f, 0.0f);
    glTranslatef(x, getHeightf(x, z), z);
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
        glTranslatef(10, 2, 0);
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


void drawTerrain() {

    // colocar aqui o código de desnho do terreno usando VBOs com TRIANGLE_STRIPS
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    for (int i = 0; i < th - 1; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * tw * 2, tw * 2);
    }

}




void renderScene(void) {

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
    if (!(abs(camX) > 127.5f || abs(camZ) > 127.5f)) {
        camY = getHeightf(camX, camZ) + 3;
        aux1();
    }
    gluLookAt(camX, camY, camZ,
		      lx,ly, lz,
			  0.0f,1.0f,0.0f);

	// put axis drawing in here
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);

	glColor3f(0, 1, 0);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);

	glColor3f(0, 0, 1);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();

	glColor3f(1, 1, 1);


	glPolygonMode(GL_FRONT, GL_LINE);
	drawTerrain();

    glColor3f(1, 1, 1);

    glPolygonMode(GL_FRONT, GL_FILL);


	// Same random seed
	srand(1);

	// Draw trees
	float x, z;

	for (int i = 0; i < 200; i++) {
		x = ((float)rand() / RAND_MAX) * 200 - 100; // Number between -100 and 100
		z = ((float)rand() / RAND_MAX) * 200 - 100;

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


	// just so that it renders something before the terrain is built
	// to erase when the terrain is ready
	//glutWireTeapot(2.0);

// End of frame
	glutSwapBuffers();
}



void processKeys(unsigned char key, int xx, int yy) {
    if(key == 'W' || key == 'w') {
        if (k<0) {
            k = -k;
        }
        aux();
    } else if(key == 'A' || key == 'a') {
        if (k>0) {
            k = -k;
        }
        produtoExterno();
    } else if(key == 'S' || key == 's') {
        if (k>0) {
            k = -k;
        }
        aux();

    } else if(key == 'D' || key == 'd') {
        if (k<0) {
            k = -k;
        }
        produtoExterno();
    } else if(key == 'C' || key == 'c') {
        alpha += 0.05;
    }
}



void processMouseButtons(int button, int state, int xx, int yy) {
	
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
}


void init() {
	// 	Load the height map "terreno.jpg"
	ilGenImages(1, &t);
	ilBindImage(t);
	
	// terreno.jpg is the image containing our height map
	ilLoadImage((ILstring)"../terreno.jpg");
	
	// convert the image to single channel per pixel
	// with values ranging between 0 and 255
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
	
	// important: check tw and th values
	// both should be equal to 256
	// if not there was an error loading the image
	// most likely the image could not be found
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	cout << "Largura: " << tw << " Altura: " << th << endl;
	
	// imageData is a LINEAR array with the pixel values
	imageData = ilGetData();

	int h = (int)th - 1, w = (int)tw - 1;
	// 	Build the vertex arrays
	for (int i = 0; i < th; i++) { 
		for (int j = 0; j < tw; j++) {
			vertexB.push_back(-w/2.0f + j);
			vertexB.push_back(getHeight(j,i));
			vertexB.push_back(-h/2.0f + i);

			vertexB.push_back(-w/2.0f + j);
			vertexB.push_back(getHeight(j, i + 1));
			vertexB.push_back(-h/2.0f + i + 1);
		}
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glGenBuffers(1, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexB.size() * sizeof(float), vertexB.data(), GL_STATIC_DRAW);

// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");
		

// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	
	glewInit();
	ilInit();
	init();	

// enter GLUT's main cycle
	glutMainLoop();
	
	return 0;
}

