#include <windows.h>		// Header File For Windows
#include <stdio.h>			// Header File For Standard Input/Output
#include <stdarg.h>			// Header File For Variable Argument Routines
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include "pages.h"
#include "font.h"

PageStuct General={
	{"Portfolio", 0.0f},
	{{0, 0.0f}, {0, 0.0f}, {0, 0.0f}, {0, 0.0f}, 
	{0, 0.0f}, {0, 0.0f}, {0, 0.0f}, {0, 0.0f}, 
	{0, 0.0f}, {0, 0.0f}, {0, 0.0f}, {0, 0.0f}, 
	{0, 0.0f}, {0, 0.0f}, {0, 0.0f}, {0, 0.0f}, 
	{0, 0.0f}, {0, 0.0f}, {0, 0.0f}, {0, 0.0f}, 
	{0, 0.0f}, {0, 0.0f}, {0, 0.0f}, {0, 0.0f}, 
	{0, 0.0f}, {0, 0.0f}, {0, 0.0f}, {0, 0.0f}, 
	{0, 0.0f}, {0, 0.0f}, {0, 0.0f}, {0, 0.0f}},
	0,
	0,
	"Data/22.bmp",
	0,
	0,
};

extern GLYPHMETRICSFLOAT gmf[256];	// Storage For Information About Our Outline Font Characters
extern GLuint  base;

void glPrintDescription(int posX, int posY, DescriptionStruct* p_st)		// Custom GL "Print" Routine
{
	float		length=0;								// Used To Find The Length Of The Text
//	char		text[256];								// Holds Our String
//	va_list		ap;										// Pointer To List Of Arguments
	unsigned int loop;

	for (loop=0;loop<(strlen(p_st->text));loop++){
		length+=gmf[p_st->text[loop]].gmfCellIncX;			// Increase Length By Each Characters Width
	}

	glTranslatef(-(length/2)+(float)posX,-(float)posY,-5.0f);
	if((p_st->cpt+= 0.05f)>= 1.0f){
		glEnable(GL_BLEND);			// Turn Blending On
		glColor4f(1.0f,1.0f,1.0f, 1.0);
	}else{
		glEnable(GL_BLEND);			// Turn Blending On
		glColor4f(p_st->cpt,p_st->cpt,p_st->cpt, 1.0);
	}
	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base);									// Sets The Base Character to 0
	glCallLists(strlen(p_st->text), GL_UNSIGNED_BYTE, p_st->text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits

	glDisable(GL_BLEND);			// Turn Blending On
}


unsigned int Helper_RenderText(char* p_text)
{
	return 0;
}

extern AUX_RGBImageRec *LoadBMP(char *Filename);

void Page_LoadPage(PageStuct* p_page)
{
	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	if(p_page->szPictureLarge){
		glGenTextures(1, &p_page->uiPictureLarge);					// Create Three Textures
		if (TextureImage[0]=LoadBMP(p_page->szPictureLarge)){

			// Create MipMapped Texture
			glBindTexture(GL_TEXTURE_2D, p_page->uiPictureLarge);
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
	}
	if(p_page->szPictureSmall){
		glGenTextures(1, &p_page->uiPictureSmall);					// Create Three Textures
		if (TextureImage[0]=LoadBMP(p_page->szPictureSmall)){

			// Create MipMapped Texture
			glBindTexture(GL_TEXTURE_2D, p_page->uiPictureSmall);
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
	}
}

extern GLfloat	z;			// Depth Into The Screen
extern float titiX[88];
extern float titiY[40];
extern GLfloat	cp1;				// X Rotation
extern GLfloat	cp2;				// Y Rotation
extern GLfloat	cp3;				// Y Rotation
extern int		cptRotX;			// Y Rotation
extern int		cptRotY;			// Y Rotation
extern GLfloat	incX;			// Y Rotation
extern GLfloat	otherX;			// Y Rotation
extern GLfloat	otherXToGo;		// Y Rotation

void Page_Render(PageStuct* p_page)
{
	glLoadIdentity();									// Reset The View
	glTranslatef(0.0f,0.0f,z);
//	glRotatef(cp3, 1.0f,0.0f,0.0f);
	glRotatef((titiX[((int)cp3)%88])/4.0f,0.0f,1.0f,0.0f);
	glRotatef((titiY[((int)cp3)%40])/4.0f,1.0f,0.0f,0.0f);
	glRotatef(otherX,0.0f,1.0f,0.0f);

	glBindTexture(GL_TEXTURE_2D, p_page->uiPictureSmall);

	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.8f, -0.8f,  1.1f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.8f, -0.8f,  1.1f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.8f,  0.8f,  1.1f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.8f,  0.8f,  1.1f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	glLoadIdentity();									// Reset The Current Modelview Matrix
	glTranslatef(0.0f,0.0f,-10.0f);						// Move One Unit Into The Screen
	glRotatef((titiX[((int)cp3)%88])/10.0f,0.0f,1.0f,0.0f);
	glRotatef((titiY[((int)cp3)%40])/10.0f,1.0f,0.0f,0.0f);

	glPrintDescription(0, -5, &p_page->szTitle);
}

int Page_HasScreenShot(PageStuct* p_page)
{
	return (int)p_page->szPictureLarge;
}

int Page_HasExecutable(PageStuct* p_page)
{
	return (int)p_page->szExecutableName;
}

int Page_HasDetailedExplanation(PageStuct* p_page)
{
	return (int)p_page->szDescription[0].text;
}