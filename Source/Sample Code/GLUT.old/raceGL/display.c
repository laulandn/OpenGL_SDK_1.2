#include "gl.h"
#include "glu.h"
#include "glut.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "map.h"
#include "sin.h"

extern GLfloat blue[];
extern float lx,ly;
extern int angle;
int ch;

void do_vakje(char c)
{
    switch(c)
    {
	case 0: ch++ ;glCallList(1); break;
	case 1: glCallList(2); break;
	case 2: glRotatef(90, 0, 1, 0); glCallList(2); break;
	case 3: glCallList(10); break;
	case 4: glRotatef(180, 0, 1, 0); glCallList(4); break;
	case 5: glRotatef(90, 0, 1, 0); glCallList(4); break;
	case 6: glRotatef(-90, 0, 1, 0); glCallList(4); break;
	case 7: glCallList(4); break;
	case 8: glScalef(-1, 1, -1); glCallList(3); break;
	case 9: glScalef(1, 1, -1); glCallList(3); break;
	case 10: glScalef(-1, 1, 1); glCallList(3); break;
	case 11: glScalef(1, 1, 1); glCallList(3); break;
	case 12: glRotatef(90, 0, 1, 0); glScalef(1, 1, 1);
		 glCallList(3); break;
	case 13: glRotatef(90, 0, 1, 0); glScalef(-1, 1, 1);
		 glCallList(3); break;
	case 14: glRotatef(90, 0, 1, 0); glScalef(1, 1, -1);
		 glCallList(3); break;
	case 15: glRotatef(90, 0, 1, 0); glScalef(-1, 1, -1);
		 glCallList(3); break;
	case 20: break; glCallList(5); break;
	case 21: glCallList(6); break;
	default: printf("Unknown character in map (%d)\n", c); break;
    }
}

void do_display()
{
    int i, j, t;
    int nx, ny;
    float tx, ty;
    set_color(blue, 20);
    glPushMatrix();
    glTranslatef(lx, 0.0, ly);
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(-50.0, 6.0, -50.0);
    glVertex3f(50.0, 6.0, -50.0);
    glVertex3f(50.0, 6.0, 50.0);
    glVertex3f(-50.0, 6.0, 50.0);
    glEnd();
    glPopMatrix();

    t = 0;
    ch = 0;
    for (i=-3.0;i<3.5;i+=1.0)
    {
	for (j=-3.0;j<3.5;j+=1.0)
	{
	    tx = COS(angle*2)*i + SIN(angle*2)*j;
	    ty = -SIN(angle*2)*i + COS(angle*2)*j;
	    glPushMatrix();
	    glTranslatef(10.0*i + floor(lx/10)*10, 0.0, 10.0*j + floor(ly/10)*10);
	    nx = (((int) (lx/10 + i) % SZ) + SZ) %SZ;
	    ny = (((int) (ly/10 + j) % SZ) + SZ) %SZ;

	    if (ty < -1.42 ||
		ty != 0 && ((tx-3/(ty+1.42)) > 1.33 || (tx+3)/(ty+1.42) < -1.33))
	    {
		/*do_vakje(20);*/
		t ++;
	    }
	    else
		do_vakje(map[ny][nx]);
	    glPopMatrix();
	}
    }

	/*
    printf("Skipped %d tiles.\n", t);
    printf("Done %d houses\n", ch);
	*/

}
