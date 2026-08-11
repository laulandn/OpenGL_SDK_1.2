#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "agl.h"

#include <Fonts.h>
#include <Events.h>
#include <Windows.h>
#include <Dialogs.h>
#include <timer.h>

#define TRI_STRIP_COUNT     10
#define TRI_STRIP_SIZE      512
#define TRI_STRIP_REPEAT    10
#define WINDOW_WIDTH        300
#define WINDOW_HEIGHT       300

#define SWAP_COUNT          10
#define GRAPHICAL_RESULTS   1

static GLubyte tmap[64] = {
	0, 255, 0, 255, 0, 255, 0, 255,
	255, 0, 255, 0, 255, 0, 255, 0,
	0, 255, 0, 255, 0, 255, 0, 255,
	255, 0, 255, 0, 255, 0, 255, 0,
	0, 255, 0, 255, 0, 255, 0, 255,
	255, 0, 255, 0, 255, 0, 255, 0,
	0, 255, 0, 255, 0, 255, 0, 255,
	255, 0, 255, 0, 255, 0, 255, 0};

static GLubyte t2map[48] = {
	255, 0, 0,  0, 255, 0,  0, 0, 255,  0, 0,   0,
	  0, 0, 0,  255, 0, 0,  0, 255, 0,  0, 0, 255,
	0, 0, 255,    0, 0, 0,  255, 0, 0,  0, 255, 0,
	0, 255, 0,  0, 0, 255,    0, 0, 0,  255, 0, 0};

static GLfloat  varray[TRI_STRIP_COUNT][TRI_STRIP_SIZE * 2];
static GLubyte  carray[TRI_STRIP_SIZE * 4];
static GLfloat  tarray[TRI_STRIP_SIZE * 2];
static GLushort indices[TRI_STRIP_SIZE];

static GLuint   str_base;

#define AGL_FONT_START  31
#define AGL_FONT_END    128

#define AGL_FONT_TYPE   kFontIDNewYork
#define AGL_FONT_SIZE   10
#define AGL_FONT_STYLE  normal

static double seconds(void)
{
   UnsignedWide _time;
   
   Microseconds(&_time);
   
   return 4294.967296 * _time.hi + 0.000001 * _time.lo;
}

static void init_str(void)
{
	str_base = glGenLists(AGL_FONT_END - AGL_FONT_START);
	
	aglUseFont(aglGetCurrentContext(), AGL_FONT_TYPE, AGL_FONT_STYLE, AGL_FONT_SIZE, AGL_FONT_START, AGL_FONT_END, str_base);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
}

static void draw_str(char *str)
{
	int len, i;
	GLubyte list_str[128];
	
	len = strlen(str);
	for(i = 0; i < len; i++) list_str[i] = str[i] - AGL_FONT_START;
	
	glListBase(str_base);
	glCallLists(len, GL_UNSIGNED_BYTE, list_str);
}

static void setup(void)
{
	GLint i, j;
	
	init_str();
	
	/* Build tri-strips */
	for(i = 0; i < TRI_STRIP_SIZE; i++) indices[i] = i;
	
	for(i = 0; i < TRI_STRIP_COUNT; i++)
	{
		for(j = 0; j < (2 * TRI_STRIP_SIZE - 3); j += 4)
		{
			GLfloat x, y1, y2;
			
			x  = -0.9f + 1.8f * ((GLfloat) j / (GLfloat) (2 * TRI_STRIP_SIZE - 4));
			y1 = -0.9f + 1.8f * ((GLfloat) i / (GLfloat) TRI_STRIP_COUNT);
			y2 = -0.9f + 1.8f * ((GLfloat) (i + 1) / (GLfloat) TRI_STRIP_COUNT);

			varray[i][j    ] = x;
			varray[i][j + 1] = y1; 

			varray[i][j + 2] = x;
			varray[i][j + 3] = y2;
		}
	}
	
	for(i = 0; i < (4 * TRI_STRIP_SIZE - 7); i += 8)
	{
		carray[i    ] = 1.0;
		carray[i + 1] = 0.0;
		carray[i + 2] = 0.0;
		carray[i + 3] = 1.0;
		
		carray[i + 4] = 0.0;
		carray[i + 5] = 1.0;
		carray[i + 6] = 0.0;
		carray[i + 7] = 1.0;
	}
	
	for(i = 0; i < (2 * TRI_STRIP_SIZE - 3); i += 4)
	{
		GLfloat s1, s2, t1, t2;
		
		s1  = 0.0f + 2.0f * ((GLfloat) i / (GLfloat) (2 * TRI_STRIP_SIZE - 4));
		s2  = 0.0f + 2.0f * ((GLfloat) (i + 1) / (GLfloat) (2 * TRI_STRIP_SIZE - 4));
		
		t1  = 0.0;
		t2  = 1.0;
		
		tarray[i    ] = s1;
		tarray[i + 1] = t1;
		
		tarray[i + 2] = s2;
		tarray[i + 3] = t2;
	}
	
	/* Coords and colors */
	glVertexPointer(2, GL_FLOAT, 8, varray[0]);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, carray);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	/* Texture unit 0 */
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	
	glTexCoordPointer(2, GL_FLOAT, 8, tarray);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
	glBindTexture(GL_TEXTURE_2D, 0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 1, 8, 8, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, tmap);
	
	glEnable(GL_TEXTURE_2D);

	/* Texture unit 1 */
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	
	glTexCoordPointer(2, GL_FLOAT, 8, tarray);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 1);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 4, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, t2map);
	
	glEnable(GL_TEXTURE_2D);
	
	/* Set projection matrix */
	glMatrixMode(GL_PROJECTION);
}

static void draw(void)
{
	GLint i, j;
	GLint swap = 1;
	char num_str[200];
	GLdouble t1 = 0.0, t2 = 0.0, t, fps = 1.0;
	GLuint ts = 0;
		
	glLockArraysEXT(0, TRI_STRIP_SIZE);
	
	t1 = seconds();
	
	if(TRI_STRIP_COUNT == 1)
	{
		while(!Button())
		{
			#if GRAPHICAL_RESULTS
				if(swap == SWAP_COUNT) glClear(GL_COLOR_BUFFER_BIT);
			#endif
			
			for(j = 0; j < TRI_STRIP_REPEAT; j++)
			{
				glDrawElements(GL_TRIANGLE_STRIP, TRI_STRIP_SIZE, GL_UNSIGNED_SHORT, indices);
			}
			
			if(swap == SWAP_COUNT)
			{
				t2 = seconds();
				t = t2 - t1;
				t1 = t2;
				
				fps = (t > 0.00001 ? (GLdouble) SWAP_COUNT / t : 1.0);
				ts = (GLuint) ((GLdouble) ((TRI_STRIP_SIZE - 2) * TRI_STRIP_COUNT * TRI_STRIP_REPEAT) * fps);
				
				#if GRAPHICAL_RESULTS
					glOrtho(0.0, WINDOW_WIDTH - 15, 0, WINDOW_HEIGHT - 15, -100.0, 100.0);
					
					glDisable(GL_TEXTURE_2D);
					
					glColor3f(1.0, 1.0, 1.0);
					glRasterPos3f(5, WINDOW_HEIGHT - 25, 0.0);
					
					sprintf(num_str, "%dx%d, Tri/Sec: %u, FPS: %f", WINDOW_WIDTH, WINDOW_HEIGHT, ts, fps);
						
					draw_str(num_str);
					
					glLoadIdentity();
					
					aglSwapBuffers(aglGetCurrentContext());
					
					glEnable(GL_TEXTURE_2D);
				#endif
				
				swap = 0;
			}
			
			swap++;
		}
	}
	else
	{
		while(!Button())
		{
			#if GRAPHICAL_RESULTS
				if(swap == SWAP_COUNT) glClear(GL_COLOR_BUFFER_BIT);
			#endif
			
			for(i = 0; i < TRI_STRIP_COUNT; i++)
			{
				glVertexPointer(2, GL_FLOAT, 8, varray[i]);
				
				for(j = 0; j < TRI_STRIP_REPEAT; j++)
				{
					glDrawElements(GL_TRIANGLE_STRIP, TRI_STRIP_SIZE, GL_UNSIGNED_SHORT, indices);
				}
			}
			
			if(swap == SWAP_COUNT)
			{
				t2 = seconds();
				t = t2 - t1;
				t1 = t2;
				
				fps = (t > 0.00001 ? (GLdouble) SWAP_COUNT / t : 1.0);
				ts = (GLuint) ((GLdouble) ((TRI_STRIP_SIZE - 2) * TRI_STRIP_COUNT * TRI_STRIP_REPEAT) * fps);

				#if GRAPHICAL_RESULTS
					glOrtho(0.0, WINDOW_WIDTH - 15, 0, WINDOW_HEIGHT - 15, -100.0, 100.0);
					
					glDisable(GL_TEXTURE_2D);
					
					glColor3f(1.0, 1.0, 1.0);
					glRasterPos3f(5, WINDOW_HEIGHT - 25, 0.0);
					
					sprintf(num_str, "%dx%d, Tri/Sec: %u, FPS: %f", WINDOW_WIDTH, WINDOW_HEIGHT, ts, fps);
						
					draw_str(num_str);
					
					glLoadIdentity();

					aglSwapBuffers(aglGetCurrentContext());
					
					glEnable(GL_TEXTURE_2D);
				#endif
				
				swap = 0;
			}
			
			swap++;
		}
	}
	
	/* One more swap for good luck */
	aglSwapBuffers(aglGetCurrentContext());
	
	#if !GRAPHICAL_RESULTS
		printf("fps : %f, ts : %u\n", fps, ts);
	#endif
}

int main(void)
{
	Rect           rect;
	CGrafPtr       win;
	GLint          attrib[] = { AGL_RGBA, AGL_DOUBLEBUFFER, AGL_NONE };
	AGLPixelFormat fmt;
	AGLContext     ctx;

	/* Initialize Macintosh system */
	InitGraf(&qd.thePort);
	InitFonts();
	FlushEvents(everyEvent, 0);
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(0L);
	InitCursor();

	/* Create a window */
	{
		int offset = 50;
		SetRect(&rect, offset, 50, offset + WINDOW_WIDTH, 50 + WINDOW_HEIGHT);
	}

	win = (CGrafPtr) NewCWindow(NULL, &rect, "\pComp Array Test", false,
								documentProc, (WindowPtr) -1L, true, 0L);
	if(win == NULL) return 1;

	ShowWindow((GrafPort *) win);

	/* Never unload a code module */
	aglConfigure(AGL_RETAIN_RENDERERS, GL_TRUE);

	/* Choose pixel format */
	fmt = aglChoosePixelFormat(NULL, 0, attrib);
	if(fmt == NULL) return 1;

	/* Create an AGL context */
	ctx = aglCreateContext(fmt, NULL);
	if(ctx == NULL) return 1;

	/* Attach the context to the window */
	if(!aglSetDrawable(ctx, win)) return 1;

	/* Make this context current */
	aglSetCurrentContext(ctx);

	/**/
	setup();
	
	/* Draw some stuff */
	draw();
	
	/* Dispose of the pixel format */
	aglDestroyPixelFormat(fmt);

	/* Make the context not current, set it's drawable to NULL, and destroy it */
	aglSetCurrentContext(NULL);
	aglSetDrawable(ctx, NULL);
	aglDestroyContext(ctx);

	/* Destroy the window */
	DisposeWindow((WindowPtr) win);

	return 0;
}

