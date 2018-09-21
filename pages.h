#ifndef _PAGES_H_
#define _PAGES_H_

typedef struct tagDescription{
	char* text;
	float cpt;
}DescriptionStruct;

typedef struct tagPage{
	DescriptionStruct	szTitle;
	DescriptionStruct	szDescription[32];
	char				*szExecutableName;
	char				*szPictureLarge;
	char				*szPictureSmall;
	unsigned int		uiPictureLarge;
	unsigned int		uiPictureSmall;
	struct tagPage	*child;
}PageStuct;

extern PageStuct General;

void glPrintDescription(int posX, int posY, DescriptionStruct* p_st);		// Custom GL "Print" Routine
void Page_Render(PageStuct* p_page);
void Page_LoadPage(PageStuct* p_page);

#endif /*_PAGES_H_*/