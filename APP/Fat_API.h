#ifndef _Fat_API_H
#define _Fat_API_H
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "ff.h"
#include "includes.h"

#define MaxFileCount    72
#define BufferSize      520

void FatFs_Open(char *filename);
void FilePath_process(char * source,char *addstring);
void ClearPath(char *path);
// void FileOpen(LISTBOX_Handle hFileBrowseListBox,TEXT_Handle hText,char *buf);
void FileOpen_Txt(char *buf);


#endif
