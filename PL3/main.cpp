#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
float alpha = M_PI / 16, beta = M_PI / 16, camera_radius = 8;
float dx = 0, dy = 0, dz = 0; 
int i = 0, j = 0;

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


void drawCylinder(float radius, float height, int slices) {

	int aux = height / 2;
	double delta = (2 * M_PI) / slices;
	double alpha = 0;

	for (int i = 0; i < slices; i++) {
		alpha = i * delta;
		float x1 = radius * sin(alpha);
		float x2 = radius * sin(alpha + delta);
		float z1 = radius * cos(alpha);
		float z2 = radius * cos(alpha + delta);
		
		glBegin(GL_TRIANGLES);

		glVertex3f(0, aux, 0);
		glVertex3f(x1, aux, z1);
		glVertex3f(x2, aux, z2);

		glVertex3f(0, -aux, 0);
		glVertex3f(x2, -aux, z2);
		glVertex3f(x1, -aux, z1);

		glVertex3f(x1, aux, z1);
		glVertex3f(x1, -aux, z1);
		glVertex3f(x2, -aux, z2);

		glVertex3f(x1, aux, z1);
		glVertex3f(x2, -aux, z2);
		glVertex3f(x2, aux, z2);

		glEnd();
	}
	
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camera_radius*cos(i*beta)*sin(j*alpha), camera_radius*sin(i*beta), camera_radius*cos(i*beta)*cos(j*alpha),
			  camera_radius * cos(i * beta) * sin(j * alpha) - camera_radius * cos(i * beta) * sin(j * alpha), 0, 0,
			  0.0f,1.0f,0.0f);

	drawCylinder(1,2,10);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

	switch (c)
	{
	case 'w':
		if (i > -8)
		{
			dy += 1;
		}
		break;
	case 'a':
		if (i < 8)
		{
			dx -= 1;
		}
		break;
	case 's':
		dy -= 1;
		break;
	case 'd':
		dx += 1;
		break;
	}
	glutPostRedisplay();

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key)
	{
	case GLUT_KEY_DOWN:
		if (i > -8) 
		{
			i -= 1;
		}
		break;
	case GLUT_KEY_UP:
		if (i < 8) 
		{
			i += 1;
		}
		break;
	case GLUT_KEY_LEFT:
		j -= 1;
		break;
	case GLUT_KEY_RIGHT:
		j += 1;
		break;
	}
	glutPostRedisplay();

}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
	glPolygonMode(GL_FRONT, GL_LINE);
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
