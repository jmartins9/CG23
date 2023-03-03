#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
float scale = 1,
	  angle=0, 
	  xtranslation=0,
	  ytranslation=0;


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


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	// put axis drawing in here
	glBegin(GL_LINES);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);

	glVertex3f(0.0f,-100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);

	glVertex3f(0.0f, 0.0f,-100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();


	// put the geometric transformations here
	glScalef(scale, scale, scale);
	glRotatef(angle, 1.0f, 0.0f, 0.0f); // angle is in degrees
	glTranslatef(xtranslation, ytranslation, 0.0f);

	// put pyramid drawing instructions here
	float width = 1.0, height = 2.0;
	glColor3f(0.0, 1.0, 1.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(width, 0.0f, width);
	glVertex3f(-width, 0.0f, -width);
	glVertex3f(width, 0.0f, -width);
	
	glVertex3f(-width, 0.0f, -width);
	glVertex3f(width, 0.0f, width);
	glVertex3f(-width, 0.0f, width);

	glVertex3f(-width, 0.0f, width);
	glVertex3f(width, 0.0f, width);
	glVertex3f(0.0f, height, 0.0f);

	glVertex3f(width, 0.0f, width);
	glVertex3f(width, 0.0f, -width);
	glVertex3f(0.0f, height, 0.0f);

	glVertex3f(width, 0.0f, -width);
	glVertex3f(-width, 0.0f, -width);
	glVertex3f(0.0f, height, 0.0f);

	glVertex3f(-width, 0.0f, width);
	glVertex3f(0.0f, height, 0.0f);
	glVertex3f(-width, 0.0f, -width);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events
void processSpecialKeys(int key_code, int xmouse, int ymouse)
{
	switch (key_code)
	{
	case GLUT_KEY_DOWN:
		ytranslation -= 0.1;
		break;
	case GLUT_KEY_UP:
		ytranslation += 0.1;
		break;
	case GLUT_KEY_LEFT:
		xtranslation -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		xtranslation += 0.1;
		break;
	}
	glutPostRedisplay();
}

void processRegularKeys(unsigned char key, int xmouse, int ymouse) 
{
	switch (key) 
	{
	case 'r':
		angle += 1.0;
		break;
	case 't':
		angle -= 1.0;
		break;
	case 's':
		scale += 0.1;
		break;
	case 'd':
		scale -= 0.1;
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
		
	// Required callback registry 
	glPolygonMode(GL_FRONT, GL_LINE);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	
	// put here the registration of the keyboard callbacks
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(processRegularKeys);


	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
