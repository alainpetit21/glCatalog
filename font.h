#ifndef _FONT_H_H
#define _FONT_H_H

void BuildFont(void);									// Build Our Bitmap Font
void KillFont(void);									// Delete The Font
void glPrint(int posX, int posY, const char *fmt, ...);	// Custom GL "Print" Routine


#endif /*_FONT_H_H*/