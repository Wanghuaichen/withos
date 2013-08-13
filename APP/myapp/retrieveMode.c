/*#include "edit.h"
#include"gui.h"
#include"wm.h"*/
#include <string.h>
#include "ff.h"
#include "motordrive.h"
#include "filerw.h"


FIL theFile;
char fnamebuf[30];
char fileReadBuf[fileReadBufMax];



void getFileList(char **fnamelist)
{
		
}
void retrieveMode(char *fname)
{
		
}
/*
unsigned retrueveName(unsigned char index)
{
		unsigned int i;
		strcpy(fnamebuf, "name");
		strcat(fnamebuf, "1");
		f_open(&theFile, fnamebuf, FA_READ);
		f_read(&theFile, fileReadBuf, 100, &i);
		f_close(&theFile);	
		return i;
}

void test(EDIT_Handle hwin)
{
		unsigned int i = 0;
		i = retrueveName(1);
		fileReadBuf[i] = '\0';
		EDIT_SetText(hwin, fileReadBuf);
}
*/