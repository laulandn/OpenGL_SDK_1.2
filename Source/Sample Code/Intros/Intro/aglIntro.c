/*
**	aglIntro.c
*/

#include "agl.h"

#include <Fonts.h>
#include <MacWindows.h>

#define WIDTH  300
#define HEIGHT 250

/*
** OpenGL Setup
*/
static AGLContext setupAGL(AGLDrawable win)
{
	GLint          attrib[] = { AGL_RGBA, AGL_NONE };
	AGLPixelFormat fmt;
	AGLContext     ctx;
	GLboolean      ok;

	/* Choose an rgb pixel format */
	fmt = aglChoosePixelFormat(NULL, 0, attrib);
	if(fmt == NULL) return NULL;

	/* Create an AGL context */
	ctx = aglCreateContext(fmt, NULL);
	if(ctx == NULL) return NULL;

	/* Attach the window to the context */
	ok = aglSetDrawable(ctx, win);
	if(!ok) return NULL;
	
	/* Make the context the current context */
	ok = aglSetCurrentContext(ctx);
	if(!ok) return NULL;

	/* Pixel format is no longer needed */
	aglDestroyPixelFormat(fmt);

	return ctx;
}

/*
** OpenGL Cleanup
*/
static void cleanupAGL(AGLContext ctx)
{
	aglSetCurrentContext(NULL);
	aglSetDrawable(ctx, NULL);
	aglDestroyContext(ctx);
}

/*
** OpenGL Drawing
*/
static void drawGL(void)
{
	/* Clear color buffer to yellow */
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	/* Draw a smooth shaded polygon */
	glBegin(GL_POLYGON);
	glColor3d(1.0, 0.0, 0.0);
	glVertex3d( 0.8,  0.8, 0.0);
	glColor3d(0.0, 1.0, 0.0);
	glVertex3d( 0.8, -0.8, 0.0);
	glColor3d(0.0, 0.0, 1.0);
	glVertex3d(-0.8, -0.8, 0.0);
	glColor3d(1.0, 0.0, 1.0);
	glVertex3d(-0.8,  0.8, 0.0);
	glEnd();

	/* Ensure completion */
	glFinish();
}

/*
** Macintosh main routine
*/
int main(int argc, char *argv[])
{
	Rect       rect;
	WindowPtr  win;
	AGLContext ctx;
	
	/* Initialize Macintosh system */
	InitGraf(&qd.thePort);
	InitFonts();
	InitWindows();

	/* Create a window */
	SetRect(&rect, 50, 50, 50 + WIDTH, 50 + HEIGHT);
	win = NewCWindow(NULL, &rect, "\pAGL Intro", false,
								plainDBox, (WindowPtr) -1L, true, 0L);
	if(win == NULL) return 1;
	
	/* Display the window */
	ShowWindow(win);

	/* Setup the OpenGL context */
	ctx = setupAGL((AGLDrawable) win);
	if(!ctx) return 1;
	
	/* Do the OpenGL drawing */
	drawGL();
	
	/* Wait until the mouse button is pressed */
	while(!Button()) {}
	
	/* Cleanup the OpenGL context */
	cleanupAGL(ctx);

	/* Cleanup */
	DisposeWindow(win);

	return 0;
}