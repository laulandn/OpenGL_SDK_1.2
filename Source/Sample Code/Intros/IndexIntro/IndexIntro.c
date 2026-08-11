/*
**	IndexIntro.c
*/

#include "agl.h"

#include <Fonts.h>
#include <MacWindows.h>
#include <math.h>

#define WIDTH  300
#define HEIGHT 250

/*
** OpenGL Setup
*/
static AGLContext setupAGL(AGLDrawable win)
{
	GLint          attrib[] = { AGL_NONE };
	AGLPixelFormat fmt;
	AGLContext     ctx;
	GLboolean      ok;
	int            i;

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
	
	/* Colormap tracking is disabled and a back buffer colormap is assigned
	** so that this intro will work with any graphics device depth. */
	aglDisable(ctx, AGL_COLORMAP_TRACKING);

	/* Assign a simple colormap of RGB stripes */
	for(i = 0; i < 256; i++)
	{
		GLint color[4];
		color[0] = i;
		color[1] = i%3 == 0 ? 65535 : 0;
		color[2] = i%3 == 1 ? 65525 : 0;
		color[3] = i%3 == 2 ? 65535 : 0;
		aglSetInteger(ctx, AGL_COLORMAP_ENTRY, color);
	}

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
	int i;

	/* Clear color buffer to yellow */
	glClearIndex(80.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	/* Disable dithering */
	glDisable(GL_DITHER);
	
	/* Draw a triangle fan */
	glBegin(GL_TRIANGLE_FAN);
	glIndexf(0.0f);
	glVertex2f( 0.0f, 0.0f);
	for(i = 0; i <= 32; i++)
	{
		GLfloat f = i * 6.28319f / 32;
		glIndexi(i);
		glVertex2f(0.9f * cosf(f), 0.9f * sinf(f));
	}
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
	win = NewCWindow(NULL, &rect, "\pIndex Intro", false,
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