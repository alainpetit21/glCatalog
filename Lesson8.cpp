#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include "font.h"		// Header File For The Glaux Library
#include "pages.h"		// Header File For The Glaux Library

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default
bool	light;				// Lighting ON/OFF
bool    blend;				// Blending OFF/ON? ( NEW )
bool	lp;					// L Pressed?
bool	fp;					// F Pressed?
bool	bp;					// B Pressed? ( NEW )

GLfloat	xrot;				// X Rotation
GLfloat	yrot;				// Y Rotation
GLfloat	cp1;				// X Rotation
GLfloat	cp2;				// Y Rotation
GLfloat	cp3;				// Y Rotation
int	cptRotX= 0;			// Y Rotation
int	cptRotY= 0;			// Y Rotation
GLfloat	incX= 0;			// Y Rotation
GLfloat	otherX= 0;			// Y Rotation
GLfloat	otherXToGo= 0;		// Y Rotation
GLfloat xspeed=0;			// X Rotation Speed
GLfloat yspeed=0;			// Y Rotation Speed
GLfloat	z=-5.0f;			// Depth Into The Screen

GLfloat LightAmbient[]=		{ 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat LightPosition[]=	{ 0.0f, 0.0f, 0.0f, 1.0f };

GLuint	filter;				// Which Filter To Use
GLuint	texture[4];			// Storage For 3 Textures
GLuint	fontID;				// Which Filter To Use

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
		return NULL;									// If Not Return NULL

	File=fopen(Filename,"r");							// Check To See If The File Exists

	if (File){
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}

int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;									// Status Indicator

	AUX_RGBImageRec *TextureImage[4];					// Create Storage Space For The Texture

	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	glGenTextures(3, &texture[0]);					// Create Three Textures
	if (TextureImage[0]=LoadBMP("Data/22.bmp")){
		Status=TRUE;									// Set The Status To TRUE

		// Create MipMapped Texture
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}
	if (TextureImage[0]){
		if (TextureImage[0]->data){
			free(TextureImage[0]->data);				// Free The Texture Image Memory
		}
		free(TextureImage[0]);							// Free The Image Structure
	}
	if (TextureImage[1]=LoadBMP("Data/Blue.bmp")){
		Status=TRUE;									// Set The Status To TRUE

		// Create MipMapped Texture
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);
	}
	if (TextureImage[1]){
		if (TextureImage[1]->data){
			free(TextureImage[1]->data);				// Free The Texture Image Memory
		}
		free(TextureImage[1]);							// Free The Image Structure
	}
	if (TextureImage[2]=LoadBMP("Data/Indigo.bmp")){
		Status=TRUE;									// Set The Status To TRUE

		// Create MipMapped Texture
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[2]->sizeX, TextureImage[2]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[2]->data);
	}
	if (TextureImage[2]){
		if (TextureImage[2]->data){
			free(TextureImage[2]->data);				// Free The Texture Image Memory
		}
		free(TextureImage[2]);							// Free The Image Structure
	}
	if (TextureImage[3]=LoadBMP("Data/Metal.bmp")){
		Status=TRUE;									// Set The Status To TRUE

		// Create MipMapped Texture
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[3]->sizeX, TextureImage[3]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[3]->data);
	}
	if (TextureImage[3]){
		if (TextureImage[3]->data){
			free(TextureImage[3]->data);				// Free The Texture Image Memory
		}
		free(TextureImage[3]);							// Free The Image Structure
	}
	return Status;										// Return The Status
}

float gWidth, gHeight;
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0){
		height=1;										// Making Height Equal One
	}

	gWidth= (float)width; gHeight= (float)height;
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	if (!LoadGLTextures())								// Jump To Texture Loading Routine
	{
		return FALSE;									// If Texture Didn't Load Return FALSE
	}

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.2f, 0.0f, 0.1f, 0.5f);				// Black Background
/*	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background*/
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
/*	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light*/
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);								// Enable Light One
	glEnable(GL_LIGHTING);								// Enable Lighting*/
	glEnable(GL_COLOR_MATERIAL);						// Enable Coloring Of Material
/*
	glEnable(GL_LIGHT1);								// Enable Default Light (Quick And Dirty)
	glEnable(GL_COLOR_MATERIAL);						// Enable Coloring Of Material
*/
/*	glColor4f(1.0f, 1.0f, 1.0f, 0.5);					// Full Brightness.  50% Alpha*/
	glBlendFunc(GL_SRC_COLOR, GL_ONE);					// Set The Blending Function For Translucency

	Page_LoadPage(&General);

	return TRUE;										// Initialization Went OK
}

float titiX[88]={
-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,   
-12,-11,-10,-9,	-8,	-7,	-6,	-5,	-4,	-3,   
-2, -1,	0,	1,	2,	3,	4,	5,	6,	7, 
8,	9,	10,	11, 12, 13, 14, 15, 16, 17, 
18, 19, 20, 21, 22, 21, 20, 19, 18, 17, 
16, 15, 14, 13, 12, 11, 10, 9,	8,	7, 
6,	5,	4,	3,	2,	1,	0,	-1,	-2, -3, 
-4,	-5,	-6, -7, -8, -9, -10,-11,-12,-13, 
-14,-15,-16,-17,-18,-19,-20,-21
};
float titiY[40]={
-10,-9,	-8,	-7,	-6,	-5,	-4,	-3,	-2,	-1,   
0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	
10,	9,	8,	7,	6,	5,	4,	3,	2,	1,	
0,	-1,	-2, -3, -4,	-5,	-6, -7, -8, -9 
};
float rot=0.0f;
#include <math.h>

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glEnable(GL_BLEND);			// Turn Blending On
	glDisable(GL_DEPTH_TEST);	// Turn Depth Testing Off

	glLoadIdentity();									// Reset The View
	glTranslatef(0.0f,0.0f,z);
	glRotatef(xrot,0.0f,0.0f,1.0f);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glColor3f(0.5f, 0.5f, 0.5f);glTexCoord2f(0.0f, (cp1/100)+0.0f); glVertex3f(-2.50f, -2.50f,  1.0f);
		glColor3f(0.5f, 0.5f, 0.5f);glTexCoord2f(1.0f, (cp1/100)+0.0f); glVertex3f( 2.50f, -2.50f,  1.0f);
		glColor3f(0.5f, 0.5f, 0.5f);glTexCoord2f(1.0f, (cp1/100)+1.0f); glVertex3f( 2.50f,  2.50f,  1.0f);
		glColor3f(0.5f, 0.5f, 0.5f);glTexCoord2f(0.0f, (cp1/100)+1.0f); glVertex3f(-2.50f,  2.50f,  1.0f);
	glEnd();

	glLoadIdentity();									// Reset The View
	glTranslatef(0.0f,0.0f,z);
	glRotatef(cp2,0.0f,0.0f,1.0f);

	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, -3.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 3.0f, -3.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 3.0f,  3.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0f,  3.0f,  1.0f);
	glEnd();

	glLoadIdentity();									// Reset The View
	glTranslatef(0.0f,0.0f,z);
	glRotatef(-cp3,0.0f,0.0f,1.0f);

	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, -3.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 3.0f, -3.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 3.0f,  3.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0f,  3.0f,  1.0f);
	glEnd();

	glDisable(GL_BLEND);			// Turn Blending On
	glEnable(GL_DEPTH_TEST);	// Turn Depth Testing Off

	glLoadIdentity();									// Reset The View
	glTranslatef(0.0f,0.0f,z);
//	glRotatef(cp3, 1.0f,0.0f,0.0f);
	glRotatef((titiX[((int)cp3)%88])/4.0f,0.0f,1.0f,0.0f);
	glRotatef((titiY[((int)cp3)%40])/4.0f,1.0f,0.0f,0.0f);
	glRotatef(otherX,0.0f,1.0f,0.0f);

	glBindTexture(GL_TEXTURE_2D, texture[3]);

	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  -1.0f);
		// Left Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,   1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,   1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  -1.0f);
		// Right Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,   1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,   1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  -1.0f);
		// Top Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -1.0f,   1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f,   1.0f);
		// Bottom Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f,   1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f,   1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f,  -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f,  -1.0f);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.8f, -0.8f,  1.1f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.8f, -0.8f,  1.1f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.8f,  0.8f,  1.1f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.8f,  0.8f,  1.1f);
		// Back Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.8f, -0.8f,  -1.1f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.8f, -0.8f,  -1.1f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.8f,  0.8f,  -1.1f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.8f,  0.8f,  -1.1f);
		// Left Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.1f, -0.8f,  -0.8f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.1f, -0.8f,   0.8f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.1f,  0.8f,   0.8f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.1f,  0.8f,  -0.8f);
		// Right Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.1f, -0.8f,  -0.8f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.1f, -0.8f,   0.8f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.1f,  0.8f,   0.8f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.1f,  0.8f,  -0.8f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	// Pulsing Colors Based On The Rotation
	Page_Render(&General);
	return TRUE;										// Keep Going
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen){
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC){
		if (!wglMakeCurrent(NULL,NULL)){
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC)){
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC)){
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd)){
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance)){
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc)){
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen){
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL){
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES){
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}else{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd))){
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd))){
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd)){
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC))){
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC)){
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL()){
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO){
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("Gl Catalog",1024,768,32,fullscreen)){
		return 0;									// Quit If Window Was Not Created
	}
	BuildFont();								// Build Our Bitmap Font

	while(!done){
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			if (msg.message==WM_QUIT){
				done=TRUE;							// If So done=TRUE
			}else{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		cp1+= 0.1f;cp2+= 0.1f;cp3+= 0.1f;

		if(otherX>otherXToGo)		otherX-= 2.0f;
		else if(otherX<otherXToGo)	otherX+= 2.0f;

		// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
		if ((active && !DrawGLScene()) || keys[VK_ESCAPE]){
			done=TRUE;							// ESC or DrawGLScene Signalled A Quit
		}else{
			SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
			if (keys[VK_PRIOR]){
				z-=0.02f;
			}
			if (keys[VK_NEXT]){
				z+=0.02f;
			}
			if ((keys[VK_RIGHT])&&(otherX==otherXToGo)){
				otherXToGo= otherX + 90;
			}
			if ((keys[VK_LEFT])&&(otherX==otherXToGo)){
				otherXToGo= otherX - 90;
			}
			if(keys[VK_RETURN])
				General.szTitle.cpt= 0.0f;
			if (keys[VK_F1]){
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("GL Catalog",1024,768,32,fullscreen)){
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}
	KillFont();							// Delete The Font

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
