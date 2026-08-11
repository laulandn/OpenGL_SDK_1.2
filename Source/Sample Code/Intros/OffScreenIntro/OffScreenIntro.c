/*
**	OffScreenIntro.c
*/

#include "agl.h"

#include <QDOffscreen.h>
#include <Fonts.h>
#include <MacWindows.h>

#define WIDTH  300
#define HEIGHT 200

/*
** OpenGL Setup
*/
static AGLContext setupAGL(void *baseaddr, GLsizei rowbytes, GLsizei pixelsize)
{
	AGLPixelFormat fmt;
	AGLContext     ctx;
	GLboolean      ok;
	GLint          attrib[] = { AGL_RGBA, AGL_PIXEL_SIZE, 0, AGL_OFFSCREEN, AGL_NONE };

	/* Set the pixel size attribute */
	attrib[2] = pixelsize;

	/* Choose an rgb pixel format */
	fmt = aglChoosePixelFormat(NULL, 0, attrib);
	if(fmt == NULL) return NULL;

	/* Create an AGL context */
	ctx = aglCreateContext(fmt, NULL);
	if(ctx == NULL) return NULL;

	/* Attach the off screen area to the context */
	ok = aglSetOffScreen(ctx, WIDTH, HEIGHT, rowbytes, baseaddr);
	if(!ok) return NULL;
	
	/* Make the context the current context */
	ok = aglSetCurrentContext(ctx);
	if(!ok) return NULL;

	/* The pixel format is no longer needed */
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
	/* Clear the color buffer */
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	/* Draw a smooth shaded polygon */
	glBegin(GL_POLYGON);
	glColor3d(1.0, 1.0, 0.0);
	glVertex3d( 0.8,  0.8, 0.0);
	glColor3d(0.0, 1.0, 1.0);
	glVertex3d( 0.8, -0.8, 0.0);
	glColor3d(1.0, 0.0, 1.0);
	glVertex3d(-0.8, -0.8, 0.0);
	glColor3d(1.0, 0.0, 0.0);
	glVertex3d(-0.8,  0.8, 0.0);
	glEnd();

	/* Ensure completion */
	glFinish();
}

/*
** Setup the front colormap
** If we're running 8 bit depth, we must set the front colormap to a 332
** map so that CopyBits will translate the colors correctly.  by default,
** a 332 map is used by the 8 bit renderers for offscreen drawables.
*/
static void set332CMap(ColorSpecPtr ctab)
{
	GLfloat a4[4];
	GLfloat a8[8];
	short i; 

	for(i = 0; i < 3; i++)
		a4[i] = 65535.0f * 0.333333333333f * i;
	a4[3] = 65535.0f;
			
	for(i = 0; i < 7; i++)
		a8[i] = 65535.0f * 0.142857142857f * i;
	a8[7] = 65535.0f;
			
	for(i = 0; i < 256; i++)
	{
		ctab[i].value     = i;
		ctab[i].rgb.blue  = (unsigned short) a4[i >> 6];
		ctab[i].rgb.green = (unsigned short) a8[(i >> 3) & 0x07];
		ctab[i].rgb.red   = (unsigned short) a8[i & 0x07];
	}
}

/*
** Macintosh main routine
*/
int main(int argc, char *argv[])
{
	Rect rect;
	WindowPtr win;
	AGLContext ctx;
	GWorldPtr g_world;
	QDErr qdErr;
	PixMapHandle pixMap;
	int rowbytes;
	void *baseaddr;
	long pixelsize;

	/* Initialize Macintosh system */
	InitGraf(&qd.thePort);
	InitFonts();
	InitWindows();

	/* Find the depth of the main screen */
	pixelsize = (*(*GetMainDevice())->gdPMap)->pixelSize;

	/* Create the offscreen gworld */
	SetRect(&rect, 0, 0, WIDTH, HEIGHT);
	qdErr = NewGWorld(&g_world, (short) pixelsize, &rect, NULL, NULL, useTempMem);
	if(qdErr || !g_world) return 1;

	/* Lock pixmap handle and get its specifications */
	pixMap = GetGWorldPixMap(g_world);
	LockPixels(pixMap);
	baseaddr = GetPixBaseAddr(pixMap);
	rowbytes = (**pixMap).rowBytes & 0x7FFF;

	/* Setup the AGL context */
	ctx = setupAGL(baseaddr, rowbytes, pixelsize);
	if(!ctx) return 1;

	/* Do the OpenGL drawing */
	drawGL();
	
	/* Cleanup the OpenGL context */
	cleanupAGL(ctx);

	/* Create a window */
	SetRect(&rect, 50, 50, 50 + WIDTH, 50 + HEIGHT);
	win = NewCWindow(NULL, &rect, "\pOffScreen Intro", false,
								plainDBox, (WindowPtr) -1L, true, 0L);
	if(win == NULL) return 1;
	ShowWindow(win);
	
	/* If we're running 8 bit depth, we must set the back colormap to a 332
	** map so that CopyBits will translate the colors correctly.  by default,
	** a 332 map is used by the 8 bit renderers for offscreen drawables. */
	if(pixelsize == 8)
		set332CMap((*(*g_world->portPixMap)->pmTable)->ctTable);
	
	/* Copy the image to the window */
	SetPort((GrafPtr) win);
	SetRect(&rect, 0, 0, 300, 200);
	CopyBits(&((GrafPtr) g_world)->portBits, &((GrafPtr) win)->portBits,
            &rect, &rect, srcCopy, NULL);

	/* Wait until the mouse button is pressed */
	while(!Button()) {}

	/* Destroy the window and GWorld */
	UnlockPixels(pixMap);
	DisposeGWorld(g_world);
	DisposeWindow((WindowPtr) win);
	
	return 0;
}
