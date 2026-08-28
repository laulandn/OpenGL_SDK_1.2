#include "gl.h"
#include "glu.h"
#include "glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "map.h"
#include "sin.h"

#define FOG

GLfloat fogcol[4]   = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightpos[4] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat lightamb[4] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat lightdif[4] = { 0.8, 0.8, 0.8, 1.0 };

float speed = 0.1;
int angle = 0, ang2 = 0;
float lx=30.0, ly=20.0, lz = 0.0, fov = 0.2;
GLboolean fog = GL_TRUE;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    do_display();
    glutSwapBuffers();
}

void myinit (void)
{
    glShadeModel (GL_SMOOTH);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);

    glClearColor(fogcol[0], fogcol[1], fogcol[2], fogcol[3]);
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdif);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glColor3f(1.0, 1.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    make_texture();
    init_lists();
    init_sin();
#ifdef FOG
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogcol);
    glFogf(GL_FOG_DENSITY, 0.01);
    glFogf(GL_FOG_START, 0.10);
    glFogf(GL_FOG_END, 50.0);
    glHint(GL_FOG_HINT, GL_NICEST);
#endif
    init_map();
}

void parsekey(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27:  exit(0);
		case 13:  speed = 0;   break;
		case 'a': fov += 0.01; break;
		case 'z': fov -= 0.01; break;
		case '1': ang2 += 1;   break;
		case '2': ang2 -= 1;   break;
		case 'f':
			fog = !fog;
			if(fog) glEnable(GL_FOG);
			else    glDisable(GL_FOG);
		break;
	}
	
	if(fov < 0.1) fov = 0.1;
}

void parsekey_special(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:	   speed += 0.03; break;
		case GLUT_KEY_DOWN:	speed -= 0.03; break;
		case GLUT_KEY_RIGHT:	angle +=3; break;
		case GLUT_KEY_LEFT:	angle -=3; break;
		case GLUT_KEY_PAGE_UP:	lz -=1; break;
		case GLUT_KEY_PAGE_DOWN:lz +=1; break;
	}
}

void SetCamera()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1333, 0.1333, -0.1, 0.1, fov, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(angle*2, 0.0, 1.0, 0.0);
    glTranslatef(lx, lz, ly);
    gluLookAt(0.0, 0.8, 0.0, 0.0, 0.8, 1.0, 0.0, 1.0, 0.0);
}

void Animate(void)
{
    lx -= speed * SIN(angle*2);
    ly += speed * COS(angle*2);

/*
    if (lx > 10*SZ)
	lx -= 10*SZ;
    if (ly > 10*SZ)
	ly -= 10*SZ;
    if (lx < 0)
	lx += 10*SZ;
    if (ly < 0)
	ly += 10*SZ;
*/

    SetCamera();
    glutPostRedisplay();
}

void myReshape(int w, int h)
{
    SetCamera();
    glViewport (0, 0, w, h);
}

int main(int argc, char *argv[])
{
	int sz;
	
	if (argc > 1)
		sz = atoi(argv[1]);
	else
		sz = 200;

	//glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(sz*1.333, sz);
	glutCreateWindow("Rally Speedway");
	glutDisplayFunc(display);
	glutKeyboardFunc(parsekey);
	glutSpecialFunc(parsekey_special);
	glutReshapeFunc(myReshape);
	glutIdleFunc(Animate);
	
	//glEnable(GL_SCISSOR_TEST);
	//glScissor(10, 10, sz*1.333 - 20, sz - 20);
	
	myinit();
	glutSwapBuffers();
	glutMainLoop();
}
