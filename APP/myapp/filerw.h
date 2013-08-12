#ifndef _filerw
#define _filerw
#include "ff.h"
#define bytesInInt (sizeof(int))
#define fileReadBufMax (maxstep*bytesInInt)
extern FIL theFile;
extern char fnamebuf[30];

void writeToFile(void *data, unsigned len, char *fname);
#endif