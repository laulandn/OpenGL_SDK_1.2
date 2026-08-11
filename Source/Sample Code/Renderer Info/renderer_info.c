#include "agl.h"
#include <stdio.h>
#include <stdlib.h>

static void PrintBufferModes(GLint v)
{
	if(v & AGL_MONOSCOPIC_BIT)   printf("            AGL_MONOSCOPIC_BIT\n");
	if(v & AGL_STEREOSCOPIC_BIT) printf("            AGL_STEREOSCOPIC_BIT\n");
	if(v & AGL_SINGLEBUFFER_BIT) printf("            AGL_SINGLEBUFFER_BIT\n");
	if(v & AGL_DOUBLEBUFFER_BIT) printf("            AGL_DOUBLEBUFFER_BIT\n");
}

static void PrintColorModes(GLint v)
{
	if(v & AGL_RGB8_BIT)         printf("            AGL_RGB8_BIT\n");
	if(v & AGL_RGB8_A8_BIT)      printf("            AGL_RGB8_A8_BIT\n");
	if(v & AGL_BGR233_BIT)       printf("            AGL_BGR233_BIT\n");
	if(v & AGL_BGR233_A8_BIT)    printf("            AGL_BGR233_A8_BIT\n");
	if(v & AGL_RGB332_BIT)       printf("            AGL_RGB332_BIT\n");
	if(v & AGL_RGB332_A8_BIT)    printf("            AGL_RGB332_A8_BIT\n");
	if(v & AGL_RGB444_BIT)       printf("            AGL_RGB444_BIT\n");
	if(v & AGL_ARGB4444_BIT)     printf("            AGL_ARGB4444_BIT\n");
	if(v & AGL_RGB444_A8_BIT)    printf("            AGL_RGB444_A8_BIT\n");
	if(v & AGL_RGB555_BIT)       printf("            AGL_RGB555_BIT\n");
	if(v & AGL_ARGB1555_BIT)     printf("            AGL_ARGB1555_BIT\n");
	if(v & AGL_RGB555_A8_BIT)    printf("            AGL_RGB555_A8_BIT\n");
	if(v & AGL_RGB565_BIT)       printf("            AGL_RGB565_BIT\n");
	if(v & AGL_RGB565_A8_BIT)    printf("            AGL_RGB565_A8_BIT\n");
	if(v & AGL_RGB888_BIT)       printf("            AGL_RGB888_BIT\n");
	if(v & AGL_ARGB8888_BIT)     printf("            AGL_ARGB8888_BIT\n");
	if(v & AGL_RGB888_A8_BIT)    printf("            AGL_RGB888_A8_BIT\n");
	if(v & AGL_RGB101010_BIT)    printf("            AGL_RGB101010_BIT\n");
	if(v & AGL_ARGB2101010_BIT)  printf("            AGL_ARGB2101010_BIT\n");
	if(v & AGL_RGB101010_A8_BIT) printf("            AGL_RGB101010_A8_BIT\n");
	if(v & AGL_RGB121212_BIT)    printf("            AGL_RGB121212_BIT\n");
	if(v & AGL_ARGB12121212_BIT) printf("            AGL_ARGB12121212_BIT\n");
	if(v & AGL_RGB161616_BIT)    printf("            AGL_RGB161616_BIT\n");
	if(v & AGL_ARGB16161616_BIT) printf("            AGL_ARGB16161616_BIT\n");
	if(v & AGL_INDEX8_BIT)       printf("            AGL_INDEX8_BIT\n");
	if(v & AGL_INDEX16_BIT)      printf("            AGL_INDEX16_BIT\n");
}

static void PrintBitModes(GLint v)
{
	if(v & AGL_0_BIT)            printf("            AGL_0_BIT\n");
	if(v & AGL_1_BIT)            printf("            AGL_1_BIT\n");
	if(v & AGL_2_BIT)            printf("            AGL_2_BIT\n");
	if(v & AGL_3_BIT)            printf("            AGL_3_BIT\n");
	if(v & AGL_4_BIT)            printf("            AGL_4_BIT\n");
	if(v & AGL_5_BIT)            printf("            AGL_5_BIT\n");
	if(v & AGL_6_BIT)            printf("            AGL_6_BIT\n");
	if(v & AGL_8_BIT)            printf("            AGL_8_BIT\n");
	if(v & AGL_10_BIT)           printf("            AGL_10_BIT\n");
	if(v & AGL_12_BIT)           printf("            AGL_12_BIT\n");
	if(v & AGL_16_BIT)           printf("            AGL_16_BIT\n");
	if(v & AGL_24_BIT)           printf("            AGL_24_BIT\n");
	if(v & AGL_32_BIT)           printf("            AGL_32_BIT\n");
	if(v & AGL_48_BIT)           printf("            AGL_48_BIT\n");
	if(v & AGL_64_BIT)           printf("            AGL_64_BIT\n");
	if(v & AGL_96_BIT)           printf("            AGL_96_BIT\n");
	if(v & AGL_128_BIT)          printf("            AGL_128_BIT\n");
}

static void PrintInfoStats(AGLRendererInfo info)
{
	GLint rv;
	
	aglDescribeRenderer(info, AGL_RENDERER_ID, &rv);
	printf("        AGL_RENDERER_ID     : 0x%X\n", rv);
	
	aglDescribeRenderer(info, AGL_OFFSCREEN, &rv);
	printf("        AGL_OFFSCREEN       : %s\n", (rv ? "GL_TRUE" : "GL_FALSE"));
	
	aglDescribeRenderer(info, AGL_FULLSCREEN, &rv);
	printf("        AGL_FULLSCREEN      : %s\n", (rv ? "GL_TRUE" : "GL_FALSE"));
	
	aglDescribeRenderer(info, AGL_WINDOW, &rv);
	printf("        AGL_WINDOW          : %s\n", (rv ? "GL_TRUE" : "GL_FALSE"));
	
	aglDescribeRenderer(info, AGL_ACCELERATED, &rv);
	printf("        AGL_ACCELERATED     : %s\n", (rv ? "GL_TRUE" : "GL_FALSE"));
	
	aglDescribeRenderer(info, AGL_ROBUST, &rv);
	printf("        AGL_ROBUST          : %s\n", (rv ? "GL_TRUE" : "GL_FALSE"));
	
	aglDescribeRenderer(info, AGL_BACKING_STORE, &rv);
	printf("        AGL_BACKING_STORE   : %s\n", (rv ? "GL_TRUE" : "GL_FALSE"));
	
	aglDescribeRenderer(info, AGL_MP_SAFE, &rv);
	printf("        AGL_MP_SAFE         : %s\n", (rv ? "GL_TRUE" : "GL_FALSE"));
	
	aglDescribeRenderer(info, AGL_COMPLIANT, &rv);
	printf("        AGL_COMPLIANT       : %s\n", (rv ? "GL_TRUE" : "GL_FALSE"));
	
	aglDescribeRenderer(info, AGL_MULTISCREEN, &rv);
	printf("        AGL_MULTISCREEN     : %s\n", (rv ? "GL_TRUE" : "GL_FALSE"));
	
	aglDescribeRenderer(info, AGL_BUFFER_MODES, &rv);
	printf("        AGL_BUFFER_MODES    : 0x%X\n", rv);
	PrintBufferModes(rv);
	
	aglDescribeRenderer(info, AGL_MIN_LEVEL, &rv);
	printf("        AGL_MIN_LEVEL       : %d\n", rv);
	
	aglDescribeRenderer(info, AGL_MAX_LEVEL, &rv);
	printf("        AGL_MAX_LEVEL       : %d\n", rv);
	
	aglDescribeRenderer(info, AGL_COLOR_MODES, &rv);
	printf("        AGL_COLOR_MODES     : 0x%X\n", rv);
	PrintColorModes(rv);
	
	aglDescribeRenderer(info, AGL_ACCUM_MODES, &rv);
	printf("        AGL_ACCUM_MODES     : 0x%X\n", rv);
	PrintColorModes(rv);
	
	aglDescribeRenderer(info, AGL_DEPTH_MODES, &rv);
	printf("        AGL_DEPTH_MODES     : 0x%X\n", rv);
	PrintBitModes(rv);
	
	aglDescribeRenderer(info, AGL_STENCIL_MODES, &rv);
	printf("        AGL_STENCIL_MODES   : 0x%X\n", rv);
	PrintBitModes(rv);
	
	aglDescribeRenderer(info, AGL_MAX_AUX_BUFFERS, &rv);
	printf("        AGL_MAX_AUX_BUFFERS : %d\n", rv);
	
	aglDescribeRenderer(info, AGL_VIDEO_MEMORY, &rv);
	printf("        AGL_VIDEO_MEMORY    : %d\n", rv);
	
	aglDescribeRenderer(info, AGL_TEXTURE_MEMORY, &rv);
	printf("        AGL_TEXTURE_MEMORY  : %d\n", rv);
}

static void CheckGetRendererInfo(GDHandle device)
{
	AGLRendererInfo info, head_info;
	GLint inum;

	head_info =  aglQueryRendererInfo(&device, 1);
	if(!head_info)
	{
		printf("aglQueryRendererInfo : Info Error\n");
		return;
	}
	
	info = head_info;
	inum = 0;
	while(info)
	{
		printf("\n    Renderer : %d\n", inum);
		PrintInfoStats(info);
		info = aglNextRendererInfo(info);
		inum++;
	}
	
	aglDestroyRendererInfo(head_info);
}

int main(int argc, char** argv)
{
	GLenum   err;
	GDHandle device;
	GLuint   dnum = 0;

  freopen ("stdout.txt", "w", stdout);

	device = GetDeviceList();
	while(device)
	{
		printf("\nDevice : %d\n", dnum);
		CheckGetRendererInfo(device);
		device = GetNextDevice(device);
		dnum++;
	}
		
	err = aglGetError();
	if(err != AGL_NO_ERROR) printf("aglGetError - %s\n", aglErrorString(err));

   return 0;
}
