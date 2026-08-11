/*
	File:		SetupGL Main Windowed.c

	Contains:	An example of the use of the SeupGL utility code for windowed applications.

	Written by:	Geoff Stahl

	Copyright:	2000 Apple Computer, Inc., All Rights Reserved

	Change History (most recent first):


	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc.
				("Apple") in consideration of your agreement to the following terms, and your
				use, installation, modification or redistribution of this Apple software
				constitutes acceptance of these terms.  If you do not agree with these terms,
				please do not use, install, modify or redistribute this Apple software.

				In consideration of your agreement to abide by the following terms, and subject
				to these terms, Apple grants you a personal, non-exclusive license, under Apple’s
				copyrights in this original Apple software (the "Apple Software"), to use,
				reproduce, modify and redistribute the Apple Software, with or without
				modifications, in source and/or binary forms; provided that if you redistribute
				the Apple Software in its entirety and without modifications, you must retain
				this notice and the following text and disclaimers in all such redistributions of
				the Apple Software.  Neither the name, trademarks, service marks or logos of
				Apple Computer, Inc. may be used to endorse or promote products derived from the
				Apple Software without specific prior written permission from Apple.  Except as
				expressly stated in this notice, no other rights or licenses, express or implied,
				are granted by Apple herein, including but not limited to any patent rights that
				may be infringed by your derivative works or by other works in which the Apple
				Software may be incorporated.

				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
				WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
				WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
				PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
				COMBINATION WITH YOUR PRODUCTS.

				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
				CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
				GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
				ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION
				OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
				(INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN
				ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    
         <2>     10/9/00    ggs     Add DrawSprocket
         <1>     3/22/00    ggs     Carbon take one

         <6>     1/26/00    ggs     Add preflight support, handle failure conditions throughout app
                                    better
         <5>     1/24/00    ggs     fixed Apple menu selection, fixed aglfont texture row length
                                    problem, fixed suspend/resume, fixed update
         <4>    12/18/99    ggs     Fix headers
         <3>    12/18/99    ggs     Added window/fullscreen toggle support and error window
         <2>    11/28/99    ggs     Standard toolbox application
         <1>    11/28/99    ggs     Initial Add
        
         <6>     9/14/99    GGS    	Corrected buffer rect handling and cleaned up code
         <5>     7/14/99    GGS    	Fixed multi-monitor window centering
         <4>     7/13/99    GGS    	Add work around for over zealous checking in single buffer DSp context attributes
         <3>     7/5/99     GGS    	Now correctly handle multi-monitor (DSp front buffer for single device; Window on top of context for multiple devices)
         <2>     5/28/99    GGS     Added better multi-monitor support, clean code, corrected blanking bug, added timing, correct pixel formats
         <1>        ?        ?      Initial build
*/


// system includes ----------------------------------------------------------


#if TARGET_API_MAC_CARBON
	#define CALL_NOT_IN_CARBON 1 // ensure DrawSprocket calls are found (must weak link and runtime check)
	#include <DrawSprocket.h>
	#undef CALL_NOT_IN_CARBON
#else
	#include <DrawSprocket.h>
#endif	

#include <Devices.h>
#include <Dialogs.h>
#include <DriverServices.h>
#include <Events.h>
#include <Gestalt.h>
#include <LowMem.h>
#include <Sound.h>
#include <TextEdit.h>
#include <ToolUtils.h>
#include <Windows.h>

// profile
//#include <Profiler.h>

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <gl.h>

// project includes ---------------------------------------------------------

#include <agl.h>
//#include "aglString.h"

// functions (internal/private) ---------------------------------------------

void CToPStr (StringPtr outString, const char *inString);
void ReportError (char * strError);
OSStatus DSpDebugStr (OSStatus error);
GLenum aglDebugStr (void);

WindowPtr SetupDSp (GDHandle *hGD, short *numDevices);
void ShutdownDSp (WindowPtr *ppWin);

AGLContext SetupAGL (GDHandle hGD, AGLDrawable win);
void CleanupAGL (AGLContext ctx);
void DrawGL(WindowPtr pWindow);

void InitToolbox(void);
void CreateGLWindow (void);
Boolean SetUp (void);
void DoMenu (SInt32 menuResult);
void DoKey (SInt8 theKey, SInt8 theCode);
void DoUpdate (WindowPtr pWindow);

#if TARGET_API_MAC_CARBON
pascal void IdleTimer (EventLoopTimerRef inTimer, void* userData);
EventLoopTimerUPP GetTimerUPP (void);
#endif

void DoEvent (void);
void CleanUp (void);

// profile wrappers
Boolean WaitNextEventWrapper (short eventMask, EventRecord *theEvent, unsigned long sleep,RgnHandle mouseRgn);
OSStatus DSpProcessEventWrapper (EventRecord *inEvent, Boolean *outEventWasProcessed);
void UpdateWrapper (EventRecord *theEvent);

// statics/globals (internal only) ------------------------------------------

// Menu defs
enum 
{
	kMenuApple = 128,
	kMenuFile = 129,
	
	kAppleAbout = 1,
	kFileQuit = 1
};

#if TARGET_API_MAC_CARBON
enum 
{
	kForegroundSleep = 10,
	kBackgroundSleep = 10000
};
EventLoopTimerRef gTimer = NULL;
#else
enum 
{
	kForegroundSleep = 0,
	kBackgroundSleep = 100
};
#endif

enum 
{
	kBitsPerPixel = 32,
	kContextWidth = 640,
	kContextHeight = 480
};

const RGBColor rgbBlack = { 0x0000, 0x0000, 0x0000 };

NumVersion gVersionDSp;
DSpContextAttributes gContextAttributes;
DSpContextReference gContext;
AGLContext gaglContext;
GLuint gFontList;
char gcstrMode [256] = "";
WindowPtr gpWindow;

SInt32 gSleepTime = kForegroundSleep;
Boolean gDone = false, gfFrontProcess = true;

// profile
#if __profile__
OSErr gProfErr = noErr;
#endif

#pragma mark -
//-----------------------------------------------------------------------------------------------------------------------

// Copy C string to Pascal string

void CToPStr (StringPtr outString, const char *inString)
{	
	unsigned char x = 0;
	do
		*(((char*)outString) + x) = *(inString + x++);
	while ((*(inString + x) != 0)  && (x < 256));
	*((char*)outString) = (char) x;									
}

// --------------------------------------------------------------------------

void ReportError (char * strError)
{
	char errMsgCStr [256];
	Str255 strErr;

	sprintf (errMsgCStr, "%s", strError); 

	// out as debug string
	CToPStr (strErr, errMsgCStr);
	DebugStr (strErr);
}

//-----------------------------------------------------------------------------------------------------------------------

OSStatus DSpDebugStr (OSStatus error)
{
	switch (error)
	{
		case noErr:
			break;
		case kDSpNotInitializedErr:
			ReportError ("DSp Error: Not initialized");
			break;
		case kDSpSystemSWTooOldErr:
			ReportError ("DSp Error: system Software too old");
			break;
		case kDSpInvalidContextErr:
			ReportError ("DSp Error: Invalid context");
			break;
		case kDSpInvalidAttributesErr:
			ReportError ("DSp Error: Invalid attributes");
			break;
		case kDSpContextAlreadyReservedErr:
			ReportError ("DSp Error: Context already reserved");
			break;
		case kDSpContextNotReservedErr:
			ReportError ("DSp Error: Context not reserved");
			break;
		case kDSpContextNotFoundErr:
			ReportError ("DSp Error: Context not found");
			break;
		case kDSpFrameRateNotReadyErr:
			ReportError ("DSp Error: Frame rate not ready");
			break;
		case kDSpConfirmSwitchWarning:
//			ReportError ("DSp Warning: Must confirm switch"); // removed since it is just a warning, add back for debugging
			return 0; // don't want to fail on this warning
			break;
		case kDSpInternalErr:
			ReportError ("DSp Error: Internal error");
			break;
		case kDSpStereoContextErr:
			ReportError ("DSp Error: Stereo context");
			break;
	}
	return error;
}

//-----------------------------------------------------------------------------------------------------------------------

// if error dump agl errors to debugger string, return error

GLenum aglDebugStr (void)
{
	GLenum err = aglGetError();
	if (AGL_NO_ERROR != err)
		ReportError ((char *)aglErrorString(err));
	return err;
}

#pragma mark -
//-----------------------------------------------------------------------------------------------------------------------

// Set up DSp screens, handles multi-monitor correctly

WindowPtr SetupDSp (GDHandle *phGD, short *numDevices)
{
	OSStatus err = noErr;
	GDHandle hDevice;
	WindowPtr pWindow;
	DSpContextAttributes foundAttributes;
	DisplayIDType displayID;
	Rect rectWin;
	RGBColor rgbSave;
	GrafPtr pGrafSave;
	*numDevices = 0;

	// check for DSp
	if ((Ptr) kUnresolvedCFragSymbolAddress == (Ptr) DSpStartup) 
		ReportError ("DSp not installed");

	if (noErr != DSpDebugStr (DSpStartup()))
		return NULL;

	if ((Ptr) kUnresolvedCFragSymbolAddress == (Ptr) DSpGetVersion) 
	{
	}
	else
		gVersionDSp = DSpGetVersion ();
 
	hDevice = DMGetFirstScreenDevice (true);								// check number of screens
	do
	{
		(*numDevices)++;
		hDevice = DMGetNextScreenDevice (hDevice, true);
	}
	while (hDevice);
			
	// Note: DSp < 1.7.3 REQUIRES the back buffer attributes even if only one buffer is required
	memset(&gContextAttributes, 0, sizeof (DSpContextAttributes));
	gContextAttributes.displayWidth			= kContextWidth;
	gContextAttributes.displayHeight		= kContextHeight;
	gContextAttributes.colorNeeds			= kDSpColorNeeds_Require;
	gContextAttributes.displayBestDepth		= kBitsPerPixel;
	gContextAttributes.backBufferBestDepth	= kBitsPerPixel;
	gContextAttributes.displayDepthMask		= kDSpDepthMask_All;
	gContextAttributes.backBufferDepthMask	= kDSpDepthMask_All;
	gContextAttributes.pageCount			= 1;								// only the front buffer is needed
	
	if (noErr != DSpDebugStr (DSpFindBestContext(&gContextAttributes, &gContext)))
		return NULL;

	if (noErr != DSpDebugStr (DSpContext_GetAttributes (gContext, &foundAttributes))) // see what we actually found
		return NULL;
		
	// reset width and height to full screen and handle our own centering
	// HWA will not correctly center less than full screen size contexts
	gContextAttributes.displayWidth 	= foundAttributes.displayWidth;
	gContextAttributes.displayHeight 	= foundAttributes.displayHeight;
	gContextAttributes.pageCount		= 1;									// only the front buffer is needed
	gContextAttributes.contextOptions	= 0 | kDSpContextOption_DontSyncVBL;	// no page flipping and no VBL sync needed

	DSpSetBlankingColor(&rgbBlack);
	if (noErr !=  DSpDebugStr (DSpContext_GetDisplayID(gContext, &displayID))) 	// get our device for future use
		return NULL;
	if (noErr !=  DMGetGDeviceByDisplayID(displayID, phGD, false)) 					// get GDHandle for ID'd device
	{
		ReportError ("DMGetGDeviceByDisplayID() had an error.");
		return NULL;
	}
	if (noErr !=  DSpDebugStr (DSpContext_Reserve ( gContext, &gContextAttributes))) // reserve our context
		return NULL;
 
	HideCursor ();

	DSpDebugStr (DSpContext_FadeGammaOut (NULL, NULL)); 							// fade display, remove for debug
	if (noErr != DSpDebugStr (DSpContext_SetState (gContext, kDSpContextState_Active))) // activate our context
		return NULL;
	
	// create a new window in our context 
	// note: OpenGL is expecting a window so it can enumerate the devices it spans, 
	// center window in our context's gdevice
	rectWin.top  = (short) ((***phGD).gdRect.top + ((***phGD).gdRect.bottom - (***phGD).gdRect.top) / 2);	  	// h center
	rectWin.top  -= (short) (kContextHeight / 2);
	rectWin.left  = (short) ((***phGD).gdRect.left + ((***phGD).gdRect.right - (***phGD).gdRect.left) / 2);	// v center
	rectWin.left  -= (short) (kContextWidth / 2);
	rectWin.right = (short) (rectWin.left + kContextWidth);
	rectWin.bottom = (short) (rectWin.top + kContextHeight);
	
	pWindow = NewCWindow (NULL, &rectWin, "\p", 0, plainDBox, (WindowPtr)-1, 0, 0);

	// paint back ground black before fade in to avoid white background flash
	ShowWindow(pWindow);
	GetPort (&pGrafSave);
	SetPortWindowPort (pWindow);
	GetForeColor (&rgbSave);
	RGBForeColor (&rgbBlack);
	{
		Rect paintRect;
#if TARGET_API_MAC_CARBON
		GetWindowPortBounds (pWindow, &paintRect);
#else
		paintRect = pWindow->portRect;
#endif // TARGET_API_MAC_CARBON
		PaintRect (&paintRect);
	}
	RGBForeColor (&rgbSave);		// ensure color is reset for proper blitting
	SetPort (pGrafSave);
	
	DSpDebugStr (DSpContext_FadeGammaIn (NULL, NULL));
	
	return pWindow;
}

//-----------------------------------------------------------------------------------------------------------------------

// clean up DSp

void ShutdownDSp (WindowPtr *ppWin)
{
	DSpContext_FadeGammaOut (NULL, NULL);
	if (*ppWin)
		DisposeWindow ((WindowPtr)*ppWin);
	*ppWin = NULL;
	DSpContext_SetState( gContext, kDSpContextState_Inactive );
	DSpContext_FadeGammaIn (NULL, NULL);
	ShowCursor ();
	DSpContext_Release (gContext);
	DSpShutdown ();
}

#pragma mark -
//-----------------------------------------------------------------------------------------------------------------------

// OpenGL Setup

AGLContext SetupAGL (GDHandle hGD, AGLDrawable win)
{
// different possible pixel format choices for different renderers 
// basics requirements are RGBA and double buffer
// OpenGL will select acclerated context if available

// software renderer
//	GLint          attrib[] = { AGL_RGBA, AGL_RENDERER_ID, AGL_RENDERER_GENERIC_ID, AGL_ALL_RENDERERS, AGL_DOUBLEBUFFER, AGL_NONE };

// any renderer
//	GLint          attrib[] = { AGL_RGBA, AGL_ALL_RENDERERS, AGL_DOUBLEBUFFER, AGL_NONE };

// OpenGL compliant HWA renderer 
	GLint          attrib[] = { AGL_RGBA, AGL_ACCELERATED, AGL_DOUBLEBUFFER, AGL_NONE };

// OpenGL compliant ATI renderer 
//	GLint          attrib[] = { AGL_RGBA, AGL_RENDERER_ID, AGL_RENDERER_ATI_ID, AGL_DOUBLEBUFFER, AGL_NONE };

// OpenGL compliant ATI HWA renderer 
//	GLint          attrib[] = { AGL_RGBA, AGL_RENDERER_ID, AGL_RENDERER_ATI_ID, AGL_ACCELERATED, AGL_DOUBLEBUFFER, AGL_NONE };

	AGLPixelFormat 	fmt;
	AGLContext     	ctx;


//	ReportError ("In GL Setup");

	if ((Ptr) kUnresolvedCFragSymbolAddress == (Ptr) aglChoosePixelFormat) // check for existance of OpenGL
	{
		ReportError ("OpenGL not installed");
		return NULL;
	}	

	if (hGD)
		fmt = aglChoosePixelFormat (&hGD, 1, attrib); 			// get an appropriate pixel format
	else
		fmt = aglChoosePixelFormat(NULL, 0, attrib);			// get an appropriate pixel format
	aglDebugStr ();
	if (NULL == fmt) 
	{
		ReportError("Could not find valid pixel format");
		return NULL;
	}
	
//	ReportError ("Have pixel format");

	ctx = aglCreateContext (fmt, NULL);							// Create an AGL context
	aglDebugStr ();
	if (NULL == ctx)
	{
		ReportError ("Could not create context");
		return NULL;
	}
	
//	ReportError ("Have context");

	if (!aglSetDrawable (ctx, win))								// attach the window to the context
	{
		ReportError ("SetDrawable failed");
		aglDebugStr ();
		return NULL;
	}

//	ReportError ("Drawable set");


	if (!aglSetCurrentContext (ctx))						// make the context the current context
	{
		aglDebugStr ();
		aglSetDrawable (ctx, NULL);
		return NULL;
	}

//	ReportError ("Current context set");

	aglDestroyPixelFormat(fmt);									// pixel format is no longer needed

	return ctx;
}

//-----------------------------------------------------------------------------------------------------------------------

// OpenGL Cleanup

void CleanupAGL(AGLContext ctx)
{
	aglSetCurrentContext(NULL);
	aglSetDrawable(ctx, NULL);
	aglDestroyContext(ctx);
}

//-----------------------------------------------------------------------------------------------------------------------

// OpenGL Drawing

void DrawGL(WindowPtr pWindow)
{
	static float rotation = 0.0;
	float f = 0.0;
	GLboolean fState = GL_FALSE;
	Rect rectPort;


//	SysBeep (30);

#if TARGET_API_MAC_CARBON
	GetWindowPortBounds (pWindow, &rectPort);
#else
	rectPort = pWindow->portRect;
#endif // TARGET_API_MAC_CARBON

	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);				// Clear color buffer to dark grey
	glClear(GL_COLOR_BUFFER_BIT);
	
	rotation += 0.5;
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glRotated (rotation, 0.0, 0.0, 1.0);
	glBegin(GL_QUADS);										// Draw a smooth shaded polygon
	glColor3d(1.0, 0.0, 0.0);
	glVertex3d(0.7, 0.7, 0.0);
	glColor3d(0.0, 1.0, 0.0);
	glVertex3d(-0.7, 0.7, 0.0);
	glColor3d(0.0, 0.0, 1.0);
	glVertex3d(-0.7, -0.7, 0.0);
	glColor3d(0.7, 0.7, 0.7);
	glVertex3d(0.7, -0.7, 0.0);
	glEnd();

	// draw info
	{
		GLint matrixMode;
		glGetIntegerv (GL_MATRIX_MODE, &matrixMode);
		glMatrixMode (GL_PROJECTION);
		glPushMatrix();
			glLoadIdentity ();
			glMatrixMode (GL_MODELVIEW);
			glPushMatrix();
				glLoadIdentity ();
				glScalef (2.0 / (rectPort.right - rectPort.left), -2.0 /  (rectPort.bottom - rectPort.top), 1.0);
				glTranslatef (-(rectPort.right - rectPort.left) / 2.0, -(rectPort.bottom - rectPort.top) / 2.0, 0.0);
				glColor3f (1.0, 1.0, 1.0);
				glRasterPos3d (10, 12, 0); 
				DrawFrameRate (gFontList);
				glRasterPos3d (10, 24, 0); 
				DrawCStringGL (gcstrMode, gFontList);
				glRasterPos3d (10, (rectPort.bottom - rectPort.top) - 15, 0); 
				DrawCStringGL ((char*) glGetString (GL_VENDOR), gFontList);
				glRasterPos3d (10, (rectPort.bottom - rectPort.top) - 3, 0); 
				DrawCStringGL ((char*) glGetString (GL_RENDERER), gFontList);
			glPopMatrix(); // GL_MODELVIEW
			glMatrixMode (GL_PROJECTION);
		glPopMatrix();
		glMatrixMode (matrixMode);
	}
	aglSwapBuffers(gaglContext);										// send swap command
}

#pragma mark -
//-----------------------------------------------------------------------------------------------------------------------

static pascal OSErr QuitAppleEventHandler( const AppleEvent *appleEvt, AppleEvent* reply, UInt32 refcon )
{
	#pragma unused (appleEvt, reply, refcon)
	gDone =  true;
	return false;
}

//-----------------------------------------------------------------------------------------------------------------------

void InitToolbox(void)
{
	OSErr err;
	long response;
	MenuHandle menu;
	
#if !TARGET_API_MAC_CARBON
	MaxApplZone ();

	InitGraf((Ptr) &qd.thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(nil);
#endif

	InitCursor();
	
// profile
#if __profile__
// 	prototype:
//		ProfilerInit (collection method, time base, num funcs, stack depth)
	// default call
	gProfErr = ProfilerInit (collectDetailed, bestTimeBase, 20, 10); // set up profiling
	// something that you may need to do (may need more memory)
//	gProfErr = ProfilerInit (collectSummary, bestTimeBase, 1000, 100); // set up profiling
// Note: application will likely run slower, but still should be useful info
	if (noErr == gProfErr)
		ProfilerSetStatus(FALSE);
#endif
	
#if !TARGET_API_MAC_CARBON
	qd.randSeed =  TickCount(); // ? Carbon Equivalent ?
#endif

	// Init Menus
	menu = NewMenu (kMenuApple, "\p\024");			// new  apple menu
	InsertMenu (menu, 0);							// add menu to end

#if !TARGET_API_MAC_CARBON
	AppendResMenu(menu, 'DRVR');
#endif
	
	menu = NewMenu (kMenuFile, "\pFile");			// new menu
	InsertMenu (menu, 0);							// add menu to end

	// insert application menus here
	
	// add quit if not under Mac OS X
	err = Gestalt (gestaltMenuMgrAttr, &response);
	if ((err == noErr) && !(response & gestaltMenuMgrAquaLayoutMask))
			AppendMenu (menu, "\pQuit/Q"); 					// add quit

	DrawMenuBar();

	err = AEInstallEventHandler( kCoreEventClass, kAEQuitApplication, NewAEEventHandlerUPP(QuitAppleEventHandler), 0, false );
	if (err != noErr)
		ExitToShell();
}

// --------------------------------------------------------------------------

Boolean SetUp (void)
{
	GDHandle hGD;
	short numDevices = 0;
	short fNum = 0;
	Rect rectPort;

	InitToolbox ();

	gpWindow = SetupDSp (&hGD, &numDevices);						// Setup DSp for OpenGL
//	ReportError ("DSp Setup Done");
	if (!gpWindow) 
		return false;
//	ReportError ("Window Valid");
#if TARGET_API_MAC_CARBON
	gaglContext = SetupAGL(hGD, GetWindowPort (gpWindow));					// Setup the OpenGL context
#else
	gaglContext = SetupAGL(hGD, (AGLDrawable) gpWindow);					// Setup the OpenGL context
#endif
///	ReportError ("GL Setup Done");
	if (!gaglContext) 
		return false;
//	ReportError ("Context Valid");
		
	#if TARGET_API_MAC_CARBON
		GetWindowPortBounds (gpWindow, &rectPort);
	#else
		rectPort = gpWindow->portRect;
	#endif // TARGET_API_MAC_CARBON
	
	if (hGD)
		sprintf (gcstrMode, "%d x %d x %d", rectPort.right, rectPort.bottom, (**(**hGD).gdPMap).pixelSize);
	else
		sprintf (gcstrMode, "%d x %d", rectPort.right, rectPort.bottom);
		
	GetFNum("\pMonaco", &fNum);									// build font
	gFontList = BuildFontGL (gaglContext, fNum, normal, 9);

	return true;
}

// --------------------------------------------------------------------------

void DoMenu (SInt32 menuResult)
{
#if !TARGET_API_MAC_CARBON
	Str255 daName;
#endif
	SInt16 theMenu;
	SInt16 theItem;
	MenuRef theMenuHandle;
		
	theMenu = HiWord(menuResult);
	theItem = LoWord(menuResult);
	theMenuHandle = GetMenuHandle(theMenu);

	switch (theMenu)
	{
		case kMenuApple:
			switch (theItem)
			{
				case kAppleAbout:
					break;
				default:
#if TARGET_API_MAC_CARBON == 0
				GetMenuItemText(GetMenuHandle(theMenu), theItem, daName);
				OpenDeskAcc(daName);
#endif				
					break;
			}
			break;
		case kMenuFile:
			switch (theItem)
			{
				case kFileQuit:
					gDone = true;
					break;
			}
			break;
	}
	HiliteMenu(0);
	DrawMenuBar();
}

// --------------------------------------------------------------------------

void DoKey (SInt8 theKey, SInt8 theCode)
{
	#pragma unused (theCode, theKey)
	// do nothing
}

// --------------------------------------------------------------------------

void DoUpdate (WindowPtr pWindow)
{
	if (pWindow)
		DrawGL (pWindow);
}

// --------------------------------------------------------------------------

Boolean WaitNextEventWrapper (short eventMask, EventRecord *theEvent, unsigned long sleep,RgnHandle mouseRgn)
{
	return WaitNextEvent (eventMask, theEvent, sleep, mouseRgn);
}

// --------------------------------------------------------------------------

OSStatus DSpProcessEventWrapper (EventRecord *inEvent, Boolean *outEventWasProcessed)
{
	return DSpProcessEvent (inEvent, outEventWasProcessed);
}

// --------------------------------------------------------------------------

void UpdateWrapper (EventRecord *theEvent)
{
	WindowRef whichWindow;
	GrafPtr pGrafSave;
	
	whichWindow = (WindowRef) theEvent->message;
	GetPort (&pGrafSave);
	SetPort((GrafPtr) GetWindowPort(whichWindow));
	BeginUpdate(whichWindow);
	DoUpdate(whichWindow);
	SetPort((GrafPtr) GetWindowPort(whichWindow));
	EndUpdate(whichWindow);
	SetPort (pGrafSave);
}

// --------------------------------------------------------------------------

#if TARGET_API_MAC_CARBON
pascal void IdleTimer (EventLoopTimerRef inTimer, void* userData)
{
	#pragma unused (inTimer, userData)
	DoUpdate (gpWindow);
}

// --------------------------------------------------------------------------

EventLoopTimerUPP GetTimerUPP (void)
{
	static EventLoopTimerUPP	sTimerUPP = NULL;
	
	if (sTimerUPP == NULL)
		sTimerUPP = NewEventLoopTimerUPP (IdleTimer);
	
	return sTimerUPP;
}
#endif

// --------------------------------------------------------------------------

void DoEvent (void)
{
	EventRecord theEvent;
	WindowRef whichWindow;
	SInt16 whatPart;
	Boolean fProcessed;
	
// profile
#if __profile__
	if (noErr == gProfErr)
		ProfilerSetStatus(TRUE); // turn on profiling
#endif

	if (WaitNextEventWrapper(everyEvent, &theEvent, gSleepTime, NULL))
	{
		DSpProcessEventWrapper (&theEvent, &fProcessed);
		if (!fProcessed)
		{
			switch (theEvent.what)
			{
				case mouseDown:
					whatPart = FindWindow (theEvent.where, &whichWindow);
					switch (whatPart)
					{
						case inContent:
							if (gpWindow != FrontWindow()) 
								SelectWindow (gpWindow);
							break;
						case inMenuBar:
						{
							SInt32 menuResult = MenuSelect (theEvent.where);
							if (HiWord (menuResult) != 0)
								DoMenu (menuResult);
						}
							break;
						case inDrag:
							{
								// full screen no drag
							}
							break;
						case inGrow:
							{
								// full screen no grow
							}
							break;
						case inGoAway:
							{
								// full screen no go away
							}
							break;
						case inZoomIn:
						case inZoomOut:
							{
								// full screen no zoom
							}
							break;
						case inSysWindow:
#if !TARGET_API_MAC_CARBON
							SystemClick(&theEvent, whichWindow);
#endif // !TARGET_API_MAC_CARBON
							break;
					}
					break;
				case keyDown:
				case autoKey:
				{
					SInt8 theKey;
					SInt8 theCode;
					theKey = theEvent.message & charCodeMask;
					theCode = (theEvent.message & keyCodeMask) >> 8;
					if ((theEvent.modifiers & cmdKey) != 0)
					{
						SInt32 menuResult = MenuKey (theKey);
						if (HiWord(menuResult) != 0)
							DoMenu (menuResult);
					}
					else
						DoKey (theKey, theCode);
				}
					break;
#if TARGET_API_MAC_CARBON
				case activateEvt:
					if (theEvent.modifiers & activeFlag)
						InstallEventLoopTimer (GetCurrentEventLoop(), 0, 0.000001, GetTimerUPP (), 0, &gTimer);
					else
					{
						RemoveEventLoopTimer(gTimer);
						gTimer = NULL;
					}
					break;
#endif // TARGET_API_MAC_CARBON
				case updateEvt:
				{
					UpdateWrapper (&theEvent);
				}
					break;
				case diskEvt:
					break;
				case osEvt:
					if (theEvent.message & 0x01000000)		//	Suspend/resume event
					{
						if (theEvent.message & 0x00000001)	//	Resume
						{
							gSleepTime = kForegroundSleep;	
							gfFrontProcess = true;
						}
						else
						{
							gSleepTime = kBackgroundSleep;	//	Suspend
							gfFrontProcess = false;
						}
					}
					break;

				case kHighLevelEvent:
					AEProcessAppleEvent (&theEvent);
					break;
			}
		}
	}
#if !TARGET_API_MAC_CARBON
	else
		DoUpdate (gpWindow);
#endif // !TARGET_API_MAC_CARBON
// profile
#if __profile__
	if (noErr == gProfErr)
		ProfilerSetStatus(FALSE); // turn profiling off again
#endif
}

// --------------------------------------------------------------------------

void CleanUp (void)
{
	MenuHandle hMenu;
	
// profile
#if __profile__
	if (noErr == gProfErr)
	{
#if TARGET_API_MAC_CARBON
		ProfilerDump ("\pGL DSp Carbon.prof");
#else
		ProfilerDump ("\pGL DSp.prof");
#endif // TARGET_API_MAC_CARBON
		ProfilerTerm ();
	}
#endif

	DeleteFontGL (gFontList);
	CleanupAGL(gaglContext);											// Cleanup the OpenGL context
	ShutdownDSp (&gpWindow);										// DSp shutdown

	hMenu = GetMenuHandle (kMenuFile);
	DeleteMenu (kMenuFile);
	DisposeMenu (hMenu);

	hMenu = GetMenuHandle (kMenuApple);
	DeleteMenu (kMenuApple);
	DisposeMenu (hMenu);

}

// --------------------------------------------------------------------------

int main (void)
{
	if (SetUp ())	
		while (!gDone) 
			DoEvent ();
	CleanUp ();
	return 0;
}