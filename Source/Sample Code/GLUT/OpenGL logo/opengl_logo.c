/*
 * Main.c
 *
 * This file is part of the openGL-logo demo.
 * (c) Henk Kok (kok@wins.uva.nl)
 *
 * Copying, redistributing, etc is permitted as long as this copyright
 * notice and the Dutch variable names :) stay in tact.
 */

#include "agl.h"
#include "glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <timer.h>

#include <Fonts.h>

GLfloat lightpos[4] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat lightamb[4] = { 0.3, 0.3, 0.3, 1.0 };
GLfloat lightdif[4] = { 0.8, 0.8, 0.8, 1.0 };
float speed=0, progress = 1;
void SetCamera(void);

extern void randomize(void);
extern void def_logo(void);
extern void draw_logo(void);

GLint w_win = 640, h_win = 480;
GLint count  = 0;

static double tnow(void)
{
   UnsignedWide time;
   
   Microseconds(&time);
   
   return 4294.967296 * time.hi + 0.000001 * time.lo;
}

static double filter(double in, double *save)
{
	static double k1 = 0.9;
	static double k2 = 0.05;

	save[3] = in;
	save[1] = save[0]*k1 + k2*(save[3] + save[2]);

	save[0]=save[1];
	save[2]=save[3];

	return(save[1]);
}

void DrawStr(char *str)
{
	static int bitmapBase = 0;
	
	if(bitmapBase == 0)
	{
		bitmapBase = glGenLists(256);
		aglUseFont(aglGetCurrentContext(), kFontIDNewYork, normal, 10, 0, 256, bitmapBase);
	}
	
	glListBase(bitmapBase);
	glCallLists(strlen(str), GL_UNSIGNED_BYTE, (GLubyte *) str);
}

void do_display (void)
{
	static double th[4] = {0.0, 0.0, 0.0, 0.0};
	static double t1 = 0.0, t2 = 0.0, t;
	char num_str[200];
     
	SetCamera();
	draw_logo();
	glFlush();
	
	t1 = t2;
	t2 = tnow();
	t = t2 - t1;
	if(t > 0.0001) t = 1.0 / t;

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glColor3f(1.0, 0.0, 0.0);

	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, w_win, 0, h_win, -10.0, 10.0);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glRasterPos2f(5.0, 5.0);
	sprintf(num_str, "%0.2f Hz, %dx%d", filter(t, th), w_win, h_win);
	DrawStr(num_str);

	glMatrixMode (GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	
	count++;
 
	glutSwapBuffers();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	do_display();
}

void myinit (void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdif);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glColor3f(1.0, 1.0, 1.0);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_NORMALIZE);
	def_logo();
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
}

/* ARGSUSED1 */
void parsekey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: exit(0); break;
	case 13: break;
	case ' ': progress = 1; randomize(); break;
	}
}

/* ARGSUSED1 */
void parsekey_special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:		break;
	case GLUT_KEY_DOWN:		break;
	case GLUT_KEY_RIGHT:	break;
	case GLUT_KEY_LEFT:		break;
	}
}

void Animate(void)
{
	speed = -0.95*speed + progress*0.05;
	if (progress > 0.0 && speed < 0.0003)
		speed = 0.0003;
	if (speed > 0.01)
		speed = 0.01;
	progress = progress - speed;
	if (progress < 0.0)
	{
		progress = 0.0;
		speed = 0;
	}
	glutPostRedisplay();
}

void myReshape(int w, int h)
{
	glMatrixMode (GL_MODELVIEW);
	glViewport (0, 0, w, h);
	glLoadIdentity();
	SetCamera();
	
	w_win = w;
	h_win = h;
}

void SetCamera(void)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glFrustum (-0.1333, 0.1333, -0.1, 0.1, 0.2, 250.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,1.5,2, 0,1.5,0, 0,1,0);
	glTranslatef(0.0, -8.0, -45.0);
	glRotatef(-progress*720, 0.0, 1.0, 0.0);
}

int main(int argc, char *argv[])
{

  freopen ("stdout.txt", "w", stdout);
  freopen ("stderr.txt", "w", stderr);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
  fprintf(stderr,"Going to glutInitWindowPosition...\n"); fflush(stderr);
	glutInitWindowPosition(50, 50);
  fprintf(stderr,"Going to glutInitWindowSize...\n"); fflush(stderr);
	glutInitWindowSize(w_win, h_win);
  fprintf(stderr,"Going to glutCreateWindow...\n"); fflush(stderr);
	glutCreateWindow("Rotating OpenGL Logo");
  fprintf(stderr,"Going to set funcs...\n"); fflush(stderr);
	glutDisplayFunc(display);
	glutKeyboardFunc(parsekey);
	glutSpecialFunc(parsekey_special);
	glutReshapeFunc(myReshape);
	glutIdleFunc(Animate);
	randomize();
  fprintf(stderr,"Going to myinit...\n"); fflush(stderr);
	myinit();
	glutSwapBuffers();
  fprintf(stderr,"Going to glutMainLoop...\n"); fflush(stderr);
	glutMainLoop();
	return 0;             /* ANSI C requires main to return int. */
}
