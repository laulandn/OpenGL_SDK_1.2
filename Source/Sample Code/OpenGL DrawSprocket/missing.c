
#include <stdio.h>

#include <agl.h>


// These are stubs, just to get the app to build
// Need to find the actual source or lib or whatever


void DrawFrameRate (GLuint gFontList)
{
  fprintf(stderr,"DrawFrameRate...not implemented\n"); fflush(stderr);
}


void DrawCStringGL (char *gcstrMode, GLuint gFontList)
{
  fprintf(stderr,"DrawCStringGL...not implemented\n"); fflush(stderr);
}


GLuint BuildFontGL (AGLContext gaglContext, short fNum, int flag, int)
{
  fprintf(stderr,"BuildFontGL...not implemented\n"); fflush(stderr);
  return 0;
}


void  DeleteFontGL (GLuint gFontList)
{
  fprintf(stderr,"DeleteFontGL...not implemented\n"); fflush(stderr);
}
