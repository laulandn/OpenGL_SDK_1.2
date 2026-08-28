#include "gl.h"
#include "glu.h"
#include "glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define STRIPES
#define STIPPLE_MIDDLE
#define HOUSE

GLfloat spec[4] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat red[4] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat green[4] = { 0.4, 0.8, 0.2, 1.0 };
GLfloat gray[4] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat white[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat blue[4] = { 0.0, 0.0, 0.6, 1.0 };
GLfloat yellow[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat brown[4] = {0.6, 0.4, 0.2 };
GLubyte road[64][64][3];

void set_color(GLfloat *rgb, float shine)
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rgb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, rgb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10);
}

void init_lists()
{
    GLfloat i,j;

    glNewList(10, GL_COMPILE);
    set_color(green, 20);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.1, 0.0);
    glVertex3f(-5.0, 0.0, -5.0);
    glVertex3f(5.0, 0.0, -5.0);
    glVertex3f(5.0, 0.0, 5.0);
    glVertex3f(-5.0, 0.0, 5.0);
    glEnd();
    glEndList();

    glNewList(1, GL_COMPILE);
    set_color(green, 20);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.1, 0.0);
    glVertex3f(-5.0, 0.0, -5.0);
    glVertex3f(5.0, 0.0, -5.0);
    glVertex3f(5.0, 0.0, 5.0);
    glVertex3f(-5.0, 0.0, 5.0);
    glEnd();


#ifdef HOUSE

#define WIDTH 3.0
#define HEIGHT 3.0
#define TOP 6.0

/* there used to be more than 1 house per square, that's why
   I have these rather silly for-loops, didn't bother to take them out. */

    set_color(brown, 10);
    for (i=0.0;i<=0.5;i+=5)
    {
	for (j=0.0;j<=0.5;j+=5)
	{
	    glBegin(GL_QUADS);
	    glNormal3f(0.0, 0.0, 1.0);
	    glVertex3f(WIDTH+i, 0.0, WIDTH+j);
	    glVertex3f(-WIDTH+i, 0.0, WIDTH+j);
	    glVertex3f(-WIDTH+i, HEIGHT, WIDTH+j);
	    glVertex3f(WIDTH+i, HEIGHT, WIDTH+j);

	    glNormal3f(0.0, 0.0, 1.0);
	    glVertex3f(WIDTH+i, 0.0, -WIDTH+j);
	    glVertex3f(-WIDTH+i, 0.0, -WIDTH+j);
	    glVertex3f(-WIDTH+i, HEIGHT, -WIDTH+j);
	    glVertex3f(WIDTH+i, HEIGHT, -WIDTH+j);

	    glNormal3f(1.0, 0.0, 0.0);
	    glVertex3f(-WIDTH+i, 0.0, WIDTH+j);
	    glVertex3f(-WIDTH+i, 0.0, -WIDTH+j);
	    glVertex3f(-WIDTH+i, HEIGHT, -WIDTH+j);
	    glVertex3f(-WIDTH+i, HEIGHT, WIDTH+j);

	    glNormal3f(1.0, 0.0, 0.0);
	    glVertex3f(WIDTH+i, 0.0, WIDTH+j);
	    glVertex3f(WIDTH+i, 0.0, -WIDTH+j);
	    glVertex3f(WIDTH+i, HEIGHT, -WIDTH+j);
	    glVertex3f(WIDTH+i, HEIGHT, WIDTH+j);

	    glEnd();

	    glBegin(GL_TRIANGLES);
	    glNormal3f(1.0, 0.0, 0.0);
	    glVertex3f(WIDTH+i, HEIGHT, WIDTH+j);
	    glVertex3f(WIDTH+i, TOP, j);
	    glVertex3f(WIDTH+i, HEIGHT, -WIDTH+j);

	    glVertex3f(-WIDTH+i, HEIGHT, WIDTH+j);
	    glVertex3f(-WIDTH+i, TOP, j);
	    glVertex3f(-WIDTH+i, HEIGHT, -WIDTH+j);

	    glEnd();
	}
    }
    set_color(red, 10);
    for (i=0;i<=0;i+=5)
    {
	for (j=0;j<=0;j+=5)
	{
	    glBegin(GL_QUADS);
	    glNormal3f(0.0, 0.707, 0.707);
	    glVertex3f(-WIDTH*1.1+i, TOP, j);
	    glVertex3f(WIDTH*1.1+i, TOP, j);
	    glVertex3f(WIDTH*1.1+i, 0.9*HEIGHT, WIDTH*1.1+j);
	    glVertex3f(-WIDTH*1.1+i, 0.9*HEIGHT, WIDTH*1.1+j);

	    glNormal3f(0.0, 0.707, -0.707);
	    glVertex3f(-WIDTH*1.1+i, TOP, j);
	    glVertex3f(WIDTH*1.1+i, TOP, j);
	    glVertex3f(WIDTH*1.1+i, 0.9*HEIGHT, -WIDTH*1.1+j);
	    glVertex3f(-WIDTH*1.1+i, 0.9*HEIGHT, -WIDTH*1.1+j);

	    glEnd();
	}
    }
#endif
    glEndList();

/* Rechte weg */
    glNewList(2, GL_COMPILE);
    set_color(green, 20);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.1, 0.0);
    glVertex3f(-5.0, 0.0, -5.0);
    glVertex3f(-2.7, 0.0, -5.0);
    glVertex3f(-2.7, 0.0, 5.0);
    glVertex3f(-5.0, 0.0, 5.0);

    glVertex3f(5.0, 0.0, -5.0);
    glVertex3f(2.7, 0.0, -5.0);
    glVertex3f(2.7, 0.0, 5.0);
    glVertex3f(5.0, 0.0, 5.0);
    glEnd();

    set_color(gray, 10);
    glBegin(GL_QUADS);
    glVertex3f(-2.5, 0.0, -5.0);
    glVertex3f(-2.5, 0.0, 5.0);
    glVertex3f(2.5, 0.0, 5.0);
    glVertex3f(2.5, 0.0, -5.0);
    glEnd();

#ifdef STRIPES
    set_color(yellow, 10);
    glBegin(GL_QUADS);

    glVertex3f(-2.7, 0.0, -5.0);
    glVertex3f(-2.5, 0.0, -5.0);
    glVertex3f(-2.5, 0.0, 5.0);
    glVertex3f(-2.7, 0.0, 5.0);

    glVertex3f(2.5, 0.0, -5.0);
    glVertex3f(2.7, 0.0, -5.0);
    glVertex3f(2.7, 0.0, 5.0);
    glVertex3f(2.5, 0.0, 5.0);

#ifdef STIPPLE_MIDDLE
    glVertex3f(-0.1, 0.01, 4.5);
    glVertex3f(-0.1, 0.01, 3.5);
    glVertex3f(0.1, 0.01, 3.5);
    glVertex3f(0.1, 0.01, 4.5);

    glVertex3f(-0.1, 0.01, 2.5);
    glVertex3f(-0.1, 0.01, 1.5);
    glVertex3f(0.1, 0.01, 1.5);
    glVertex3f(0.1, 0.01, 2.5);

    glVertex3f(-0.1, 0.01, 0.5);
    glVertex3f(-0.1, 0.01, -0.5);
    glVertex3f(0.1, 0.01, -0.5);
    glVertex3f(0.1, 0.01, 0.5);

    glVertex3f(-0.1, 0.01, -1.5);
    glVertex3f(-0.1, 0.01, -2.5);
    glVertex3f(0.1, 0.01, -2.5);
    glVertex3f(0.1, 0.01, -1.5);

    glVertex3f(-0.1, 0.01, -4.5);
    glVertex3f(-0.1, 0.01, -3.5);
    glVertex3f(0.1, 0.01, -3.5);
    glVertex3f(0.1, 0.01, -4.5);
#endif
    glEnd();
#endif
    glEndList();

#define R10 -3.55
#define R11 -2.6
#define R12 -2.0
#define R13 -1.4
#define R14 -1.2
#define R15 -1.4
#define R16 -1.7
#define R17 -2.0
#define R18 -2.3
#define R19 -2.4

#define R20 3.55
#define R21 4.2
#define R22 4.2
#define R23 4.0
#define R24 3.8
#define R25 3.3
#define R26 3.0
#define R27 2.7
#define R28 2.55
#define R29 2.5

    glNewList(3, GL_COMPILE);
    set_color(green, 20);
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0.0, 1.1, 0.0);
    glVertex3f(-5.0, 0.0, 5.0);
    glVertex3f(R10-0.2, 0.0, 5.0);
    glVertex3f(-5.0, 0.0, 4.0);
    glVertex3f(R11-0.2, 0.0, 4.0);
    glVertex3f(-5.0, 0.0, 3.0);
    glVertex3f(R12-0.2, 0.0, 3.0);
    glVertex3f(-5.0, 0.0, 2.0);
    glVertex3f(R13-0.2, 0.0, 2.0);
    glVertex3f(-5.0, 0.0, 1.0);
    glVertex3f(R14-0.2, 0.0, 1.0);
    glVertex3f(-5.0, 0.0, 0.0);
    glVertex3f(R15-0.2, 0.0, 0.0);
    glVertex3f(-5.0, 0.0, -1.0);
    glVertex3f(R16-0.2, 0.0, -1.0);
    glVertex3f(-5.0, 0.0, -2.0);
    glVertex3f(R17-0.2, 0.0, -2.0);
    glVertex3f(-5.0, 0.0, -3.0);
    glVertex3f(R18-0.2, 0.0, -3.0);
    glVertex3f(-5.0, 0.0, -4.0);
    glVertex3f(R19-0.2, 0.0, -4.0);
    glVertex3f(-5.0, 0.0, -5.0);
    glVertex3f(-2.7, 0.0, -5.0);
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glVertex3f(5.0, 0.0, 5.0);
    glVertex3f(R20+0.2, 0.0, 5.0);
    glVertex3f(5.0, 0.0, 4.0);
    glVertex3f(R21+0.2, 0.0, 4.0);
    glVertex3f(5.0, 0.0, 3.0);
    glVertex3f(R22+0.2, 0.0, 3.0);
    glVertex3f(5.0, 0.0, 2.0);
    glVertex3f(R23+0.2, 0.0, 2.0);
    glVertex3f(5.0, 0.0, 1.0);
    glVertex3f(R24+0.2, 0.0, 1.0);
    glVertex3f(5.0, 0.0, 0.0);
    glVertex3f(R25+0.2, 0.0, 0.0);
    glVertex3f(5.0, 0.0, -1.0);
    glVertex3f(R26+0.2, 0.0, -1.0);
    glVertex3f(5.0, 0.0, -2.0);
    glVertex3f(R27+0.2, 0.0, -2.0);
    glVertex3f(5.0, 0.0, -3.0);
    glVertex3f(R28+0.2, 0.0, -3.0);
    glVertex3f(5.0, 0.0, -4.0);
    glVertex3f(R29+0.2, 0.0, -4.0);
    glVertex3f(5.0, 0.0, -5.0);
    glVertex3f(2.7, 0.0, -5.0);
    glEnd();

    set_color(gray, 10);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(R10, 0.0, 5.0);
    glVertex3f(R20, 0.0, 5.0);
    glVertex3f(R11, 0.0, 4.0);
    glVertex3f(R21, 0.0, 4.0);
    glVertex3f(R12, 0.0, 3.0);
    glVertex3f(R22, 0.0, 3.0);
    glVertex3f(R13, 0.0, 2.0);
    glVertex3f(R23, 0.0, 2.0);
    glVertex3f(R14, 0.0, 1.0);
    glVertex3f(R24, 0.0, 1.0);
    glVertex3f(R15, 0.0, 0.0);
    glVertex3f(R25, 0.0, 0.0);
    glVertex3f(R16, 0.0, -1.0);
    glVertex3f(R26, 0.0, -1.0);
    glVertex3f(R17, 0.0, -2.0);
    glVertex3f(R27, 0.0, -2.0);
    glVertex3f(R18, 0.0, -3.0);
    glVertex3f(R28, 0.0, -3.0);
    glVertex3f(R19, 0.0, -4.0);
    glVertex3f(R29, 0.0, -4.0);
    glVertex3f(-2.5, 0.0, -5.0);
    glVertex3f(2.5, 0.0, -5.0);
    glEnd();

#ifdef STRIPES
    set_color(yellow, 10);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(R10, 0.0, 5.0);
    glVertex3f(R10-0.2, 0.0, 5.0);
    glVertex3f(R11, 0.0, 4.0);
    glVertex3f(R11-0.2, 0.0, 4.0);
    glVertex3f(R12, 0.0, 3.0);
    glVertex3f(R12-0.2, 0.0, 3.0);
    glVertex3f(R13, 0.0, 2.0);
    glVertex3f(R13-0.2, 0.0, 2.0);
    glVertex3f(R14, 0.0, 1.0);
    glVertex3f(R14-0.2, 0.0, 1.0);
    glVertex3f(R15, 0.0, 0.0);
    glVertex3f(R15-0.2, 0.0, 0.0);
    glVertex3f(R16, 0.0, -1.0);
    glVertex3f(R16-0.2, 0.0, -1.0);
    glVertex3f(R17, 0.0, -2.0);
    glVertex3f(R17-0.2, 0.0, -2.0);
    glVertex3f(R18, 0.0, -3.0);
    glVertex3f(R18-0.2, 0.0, -3.0);
    glVertex3f(R19, 0.0, -4.0);
    glVertex3f(R19-0.2, 0.0, -4.0);
    glVertex3f(-2.5, 0.0, -5.0);
    glVertex3f(-2.7, 0.0, -5.0);
    glEnd();

    glBegin(GL_QUAD_STRIP);
    glVertex3f(R20, 0.0, 5.0);
    glVertex3f(R20+0.2, 0.0, 5.0);
    glVertex3f(R21, 0.0, 4.0);
    glVertex3f(R21+0.2, 0.0, 4.0);
    glVertex3f(R22, 0.0, 3.0);
    glVertex3f(R22+0.2, 0.0, 3.0);
    glVertex3f(R23, 0.0, 2.0);
    glVertex3f(R23+0.2, 0.0, 2.0);
    glVertex3f(R24, 0.0, 1.0);
    glVertex3f(R24+0.2, 0.0, 1.0);
    glVertex3f(R25, 0.0, 0.0);
    glVertex3f(R25+0.2, 0.0, 0.0);
    glVertex3f(R26, 0.0, -1.0);
    glVertex3f(R26+0.2, 0.0, -1.0);
    glVertex3f(R27, 0.0, -2.0);
    glVertex3f(R27+0.2, 0.0, -2.0);
    glVertex3f(R28, 0.0, -3.0);
    glVertex3f(R28+0.2, 0.0, -3.0);
    glVertex3f(R29, 0.0, -4.0);
    glVertex3f(R29+0.2, 0.0, -4.0);
    glVertex3f(2.5, 0.0, -5.0);
    glVertex3f(2.7, 0.0, -5.0);
    glEnd();

#ifdef STIPPLE_MIDDLE
    glBegin(GL_QUADS);
    glVertex3f((R10+R11+R20+R21)/4 + 0.1, 0.01, 4.5);
    glVertex3f((R10+R11+R20+R21)/4 - 0.1, 0.01, 4.5);
    glVertex3f((R11+R12+R21+R22)/4 - 0.1, 0.01, 3.5);
    glVertex3f((R11+R12+R21+R22)/4 + 0.1, 0.01, 3.5);
    glVertex3f((R12+R12+R22+R22)/4 + 0.1, 0.01, 2.5);
    glVertex3f((R12+R12+R22+R22)/4 - 0.1, 0.01, 2.5);
    glVertex3f((R13+R13+R23+R23)/4 - 0.1, 0.01, 1.5);
    glVertex3f((R13+R13+R23+R23)/4 + 0.1, 0.01, 1.5);
    glVertex3f((R14+R14+R24+R24)/4 + 0.1, 0.01, 0.5);
    glVertex3f((R14+R14+R24+R24)/4 - 0.1, 0.01, 0.5);
    glVertex3f((R15+R15+R25+R25)/4 - 0.1, 0.01, -0.5);
    glVertex3f((R15+R15+R25+R25)/4 + 0.1, 0.01, -0.5);
    glVertex3f((R16+R16+R26+R26)/4 + 0.1, 0.01, -1.5);
    glVertex3f((R16+R16+R26+R26)/4 - 0.1, 0.01, -1.5);
    glVertex3f((R17+R17+R27+R27)/4 - 0.1, 0.01, -2.5);
    glVertex3f((R17+R17+R27+R27)/4 + 0.1, 0.01, -2.5);
    glVertex3f((R18+R18+R28+R28)/4 + 0.1, 0.01, -3.5);
    glVertex3f((R18+R18+R28+R28)/4 - 0.1, 0.01, -3.5);
    glVertex3f((R19+R19+R29+R29)/4 - 0.1, 0.01, -4.5);
    glVertex3f((R19+R19+R29+R29)/4 + 0.1, 0.01, -4.5);
    glEnd();
#endif
#endif
    glEndList();

/* schuin stukje */
    glNewList(4, GL_COMPILE);
    set_color(green, 20);
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0, 1.1, 0.0);
    glVertex3f(-3.75, 0.0, -5.0);
    glVertex3f(-5.0, 0.0, -3.75);
    glVertex3f(-5.0, 0.0, -5.0);
    glEnd();
    glBegin(GL_QUAD_STRIP);
    glVertex3f(5.0, 0.0, -5.0);
    glVertex3f(3.75, 0.0, -5.0);
    glVertex3f(5.0, 0.0, 3.75);
    glVertex3f(-5.0, 0.0, 3.75);
    glVertex3f(5.0, 0.0, 5.0);
    glVertex3f(-5.0, 0.0, 5.0);
    glEnd();

    set_color(gray, 10);
    glBegin(GL_QUADS);
    glVertex3f(3.55, 0.0, -5.0);
    glVertex3f(-5.0, 0.0, 3.55);
    glVertex3f(-5.0, 0.0, -3.55);
    glVertex3f(-3.55, 0.0, -5.0);
    glEnd();
#ifdef STRIPES
    set_color(yellow, 10);
    glBegin(GL_QUADS);
    glVertex3f(3.75, 0.0, -5.0);
    glVertex3f(3.55, 0.0, -5.0);
    glVertex3f(-5.0, 0.0, 3.55);
    glVertex3f(-5.0, 0.0, 3.75);
    glVertex3f(-3.55, 0.0, -5.0);
    glVertex3f(-3.75, 0.0, -5.0);
    glVertex3f(-5.0, 0.0, -3.75);
    glVertex3f(-5.0, 0.0, -3.55);

#ifdef STIPPLE_MIDDLE
/* 0.42-1.25 : 2.08-2.92 : 3.75-4.58 */
    glVertex3f(-0.49, 0.01, -4.65);
    glVertex3f(-0.35, 0.01, -4.51);
    glVertex3f(-1.18, 0.01, -3.68);
    glVertex3f(-1.32, 0.01, -3.82);

    glVertex3f(-2.01, 0.01, -2.85);
    glVertex3f(-2.15, 0.01, -2.99);
    glVertex3f(-2.99, 0.01, -2.15);
    glVertex3f(-2.85, 0.01, -2.01);

    glVertex3f(-4.65, 0.01, -0.49);
    glVertex3f(-4.51, 0.01, -0.35);
    glVertex3f(-3.68, 0.01, -1.18);
    glVertex3f(-3.82, 0.01, -1.32);
#endif    
    glEnd();
#endif

	glEndList();
}

void make_texture()
{
    int i,j;
    for (i=0;i<64;i++)
    {
	for (j=0;j<64;j++)
	{
	    road[i][j][0] = 32;
	    road[i][j][1] = 192;
	    road[i][j][2] = 64;
	}
	for (j=16;j<48;j++)
	{
	    road[i][j][0] = 128;
	    road[i][j][1] = 128;
	    road[i][j][2] = 128;
	}
	road[i][15][0] = 255;
	road[i][15][1] = 255;
	road[i][15][2] = 255;
	road[i][14][0] = 255;
	road[i][14][1] = 255;
	road[i][14][2] = 255;
	road[i][48][0] = 255;
	road[i][48][1] = 255;
	road[i][48][2] = 255;
	road[i][49][0] = 255;
	road[i][49][1] = 255;
	road[i][49][2] = 255;
	if ((i%16) > 4 && (i%16) < 12)
	{
	    road[i][33][0] = 255;
	    road[i][33][1] = 255;
	    road[i][33][2] = 255;
	    road[i][32][0] = 255;
	    road[i][32][1] = 255;
	    road[i][32][2] = 255;
	}
    }
}
